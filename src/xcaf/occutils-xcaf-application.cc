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

// -----------------------------------------------------------------------------

TDF_Label ExtendedXCAFApplication::AddShape(
    const TopoDS_Shape& shape, const std::string& shapeName) const {
  TDF_Label shapeLabel = this->GetShapeTool()->NewShape();
  this->GetShapeTool()->SetShape(shapeLabel, shape);

  if (!shapeName.empty()) {
    TDataStd_Name::Set(shapeLabel, shapeName.c_str());
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label ExtendedXCAFApplication::AddShapeWithProps(
    const TopoDS_Shape& shape, const XCAFShapeProperties& props) {
  TDF_Label shapeLabel = this->GetShapeTool()->NewShape();
  this->GetShapeTool()->SetShape(shapeLabel, shape);

  // Set color if specified
  if (props.HasColor()) {
    this->GetColorTool()->SetColor(shapeLabel, props.GetColor(),
                                   props.GetColorType());
  }

  // Add name if specified
  if (!props.GetName().empty()) {
    TDataStd_Name::Set(shapeLabel, props.GetName().c_str());
  }

  // Add material if specified
  if (!props.GetMaterial().IsNull()) {
    TDF_Label materialLabel = FindOrCreateMaterial(props.GetMaterial());
    this->internals->materialTool->SetMaterial(shapeLabel, materialLabel);
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label ExtendedXCAFApplication::FindOrCreateMaterial(
    const XCAFMaterial& material) {
  // Check if the material already exists
  TDF_LabelSequence materials;
  this->internals->materialTool->GetMaterialLabels(materials);
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

  TDF_Label newMaterialLabel = this->internals->materialTool->AddMaterial(
      name, description, material.GetDensity(), densName, densValType);

  return newMaterialLabel;
}

// -----------------------------------------------------------------------------

std::vector<TDF_Label> ExtendedXCAFApplication::GetMaterials() const {
  TDF_LabelSequence materials;
  this->internals->materialTool->GetMaterialLabels(materials);

  std::vector<TDF_Label> materialLabels;
  for (Standard_Integer i = 1; i <= materials.Length(); i++) {
    materialLabels.push_back(materials.Value(i));
  }
  return materialLabels;
}

//-----------------------------------------------------------------------------

TopoDS_Shape ExtendedXCAFApplication::GetShape(const TDF_Label& label) {
  if (label.IsNull()) return {};

  return XCAFDoc_ShapeTool::GetShape(label);
}

//-----------------------------------------------------------------------------

TopoDS_Shape ExtendedXCAFApplication::GetOneShape() const {
  // Get all parts.
  TDF_LabelSequence labels;
  this->GetShapeTool()->GetFreeShapes(labels);

  if (!labels.Length()) return {};

  if (labels.Length() == 1) return XCAFDoc_ShapeTool::GetShape(labels.First());

  // Put everything into compound and return.
  TopoDS_Compound C;
  BRep_Builder B;
  B.MakeCompound(C);
  for (int i = 1; i <= labels.Length(); ++i) {
    TopoDS_Shape S = XCAFDoc_ShapeTool::GetShape(labels(i));
    B.Add(C, S);
  }
  return C;
}

// -----------------------------------------------------------------------------

void ExtendedXCAFApplication::ResetColors() const {
  // Get color tool.
  Handle(XCAFDoc_ColorTool) CT = this->GetColorTool();

  TDF_LabelSequence colorLabs;
  CT->GetColors(colorLabs);

  for (TDF_LabelSequence::Iterator lit(colorLabs); lit.More(); lit.Next()) {
    TDF_Label colorLab = lit.Value();
    CT->RemoveColor(colorLab);
  }
}

// -----------------------------------------------------------------------------

Handle(TDocStd_Document) & ExtendedXCAFApplication::ChangeDocument() {
  return this->internals->document;
}

// -----------------------------------------------------------------------------

const Handle(TDocStd_Document) & ExtendedXCAFApplication::GetDocument() const {
  return this->internals->document;
}

// -----------------------------------------------------------------------------

Handle(XCAFDoc_ShapeTool) ExtendedXCAFApplication::GetShapeTool() const {
  return this->internals->shapeTool;
}

// -----------------------------------------------------------------------------

Handle(XCAFDoc_ColorTool) ExtendedXCAFApplication::GetColorTool() const {
  return this->internals->colorTool;
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

  if (!reader.Transfer(internals->document)) {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

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
