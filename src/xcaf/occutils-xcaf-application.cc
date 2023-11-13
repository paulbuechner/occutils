#include "occutils/xcaf/occutils-xcaf-application.h"

#include <Interface_Static.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <TDataStd_Name.hxx>
#include <TDocStd_Application.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFApp_Application.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DimTolTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_LayerTool.hxx>
#include <XCAFDoc_MaterialTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <filesystem>
#include <memory>
#include <vector>

namespace occutils::xcaf {

/**
 * Internal state
 */
struct ExtendedXCAFApplicationInternals {
  ExtendedXCAFApplicationInternals() {
    // Create document
    application = XCAFApp_Application::GetApplication();
    application->NewDocument("MDTV-XCAF", document);
    // Get tools
    shapeTool = XCAFDoc_DocumentTool::ShapeTool(document->Main());
    colorTool = XCAFDoc_DocumentTool::ColorTool(document->Main());
    materialTool = XCAFDoc_DocumentTool::MaterialTool(document->Main());
    layerTool = XCAFDoc_DocumentTool::LayerTool(document->Main());
    dimTolTool = XCAFDoc_DocumentTool::DimTolTool(document->Main());
  }
  // Base attributes
  Handle(TDocStd_Document) document;
  Handle(TDocStd_Application) application;
  // Tools
  Handle(XCAFDoc_ShapeTool) shapeTool;
  Handle(XCAFDoc_ColorTool) colorTool;
  Handle(XCAFDoc_MaterialTool) materialTool;
  Handle(XCAFDoc_LayerTool) layerTool;
  Handle(XCAFDoc_DimTolTool) dimTolTool;
};

// -----------------------------------------------------------------------------

ExtendedXCAFApplication::ExtendedXCAFApplication() {
  internals = std::make_unique<ExtendedXCAFApplicationInternals>();
}

ExtendedXCAFApplication::~ExtendedXCAFApplication() {
  // Make sure to close the document
  internals->application->Close(internals->document);
}

TDF_Label ExtendedXCAFApplication::AddShape(const TopoDS_Shape& shape,
                                            const std::string& shapeName) {
  TDF_Label shapeLabel = internals->shapeTool->NewShape();
  internals->shapeTool->SetShape(shapeLabel, shape);

  if (!shapeName.empty()) {
    TDataStd_Name::Set(shapeLabel, shapeName.c_str());
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label ExtendedXCAFApplication::AddShapeWithProps(
    const TopoDS_Shape& shape, const XCAFShapeProperties& props) {
  TDF_Label shapeLabel = internals->shapeTool->NewShape();
  internals->shapeTool->SetShape(shapeLabel, shape);

  // Set color if specified
  if (props.HasColor()) {
    internals->colorTool->SetColor(shapeLabel, props.GetColor(),
                                   props.GetColorType());
  }

  // Add name if specified
  if (!props.GetName().empty()) {
    TDataStd_Name::Set(shapeLabel, props.GetName().c_str());
  }

  // Add material if specified
  if (!props.GetMaterial().IsNull()) {
    TDF_Label materialLabel = FindOrCreateMaterial(props.GetMaterial());
    internals->materialTool->SetMaterial(shapeLabel, materialLabel);
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label ExtendedXCAFApplication::FindOrCreateMaterial(
    const XCAFMaterial& material) {
  // Check if the material already exists
  TDF_LabelSequence materials;
  internals->materialTool->GetMaterialLabels(materials);
  for (Standard_Integer i = 1; i <= materials.Length(); i++) {
    TDF_Label materialLabel = materials.Value(i);
    Handle(TCollection_HAsciiString) name;
    Handle(TCollection_HAsciiString) description;
    Standard_Real density;
    Handle(TCollection_HAsciiString) densName;
    Handle(TCollection_HAsciiString) densValType;

    if (XCAFDoc_MaterialTool::GetMaterial(materialLabel, name, description,
                                          density, densName, densValType)) {
      XCAFMaterial extMaterial(name->ToCString(), description->ToCString(),
                               density, densName->ToCString(),
                               densValType->ToCString());
      if (extMaterial == material) {  // Found existing material
        return materialLabel;
      }
    }
  }

  // Material not found, create a new one
  Handle(TCollection_HAsciiString) name =
      new TCollection_HAsciiString(material.GetName().c_str());
  Handle(TCollection_HAsciiString) description =
      new TCollection_HAsciiString(material.GetDescription().c_str());
  Handle(TCollection_HAsciiString) densName =
      new TCollection_HAsciiString(material.GetDensityName().c_str());
  Handle(TCollection_HAsciiString) densValType =
      new TCollection_HAsciiString(material.GetDensityValueType().c_str());

  TDF_Label newMaterialLabel = internals->materialTool->AddMaterial(
      name, description, material.GetDensity(), densName, densValType);

  return newMaterialLabel;
}

// -----------------------------------------------------------------------------

std::vector<TDF_Label> ExtendedXCAFApplication::GetMaterials() const {
  TDF_LabelSequence materials;
  internals->materialTool->GetMaterialLabels(materials);

  std::vector<TDF_Label> materialLabels;
  for (Standard_Integer i = 1; i <= materials.Length(); i++) {
    materialLabels.push_back(materials.Value(i));
  }
  return materialLabels;
}

// -----------------------------------------------------------------------------

bool ExtendedXCAFApplication::ReadSTEP(const std::string& filename) {
  STEPCAFControl_Reader reader;
  reader.SetColorMode(true);
  reader.SetLayerMode(true);
  reader.SetNameMode(true);
  reader.SetMatMode(true);
  reader.SetPropsMode(true);

  if (auto status = reader.ReadFile(filename.c_str());
      status != IFSelect_RetDone) {
    return false;
  }

  return reader.Transfer(internals->document);
}

bool ExtendedXCAFApplication::WriteSTEP(const std::string& filename,
                                        const std::string& exportUnit) {
  // Parse directory from filename
  std::filesystem::path filePath(filename);

  // Check if directory exists
  if (std::filesystem::path dirPath = filePath.parent_path();
      !std::filesystem::exists(dirPath) &&
      !std::filesystem::create_directories(dirPath)) {
    // Handle error: unable to create directory
    return false;
  }

  // Set STEP export parameters
  Interface_Static::SetCVal("write.step.unit", exportUnit.c_str());
  STEPCAFControl_Writer writer;
  writer.SetMaterialMode(true);
  writer.SetDimTolMode(true);
  writer.SetLayerMode(true);
  writer.SetPropsMode(true);
  writer.SetColorMode(true);

  // Write the STEP file
  return writer.Perform(internals->document, filename.c_str());
}

}  // namespace occutils::xcaf
