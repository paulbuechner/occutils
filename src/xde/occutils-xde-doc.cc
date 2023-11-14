/***************************************************************************
 *   Created on: 15 Oct 2023                                               *
 ***************************************************************************
 *   Copyright (c) 2022, Paul Buechner                                     *
 *                                                                         *
 *   This file is part of the OCCUtils library.                            *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the Apache License version 2.0 as        *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 ***************************************************************************/

//---------------------------------------------------------------------------
// This file incorporates work covered by the following copyright and
// permission notice:
//
// Created on: 22 September 2020 (*)
// Created by: Sergey SLYADNEV
//-----------------------------------------------------------------------------
// Copyright (c) 2020-present, Sergey Slyadnev
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright
//      notice, this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//    * Neither the name of the copyright holder(s) nor the
//      names of all contributors may be used to endorse or promote products
//      derived from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------

#include "occutils/xde/occutils-xde-doc.h"

#include <APIHeaderSection_MakeHeader.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <Interface_Static.hxx>
#include <STEPCAFControl_Reader.hxx>
#include <STEPCAFControl_Writer.hxx>
#include <STEPControl_Controller.hxx>
#include <TDF_ChildIterator.hxx>
#include <TDF_Tool.hxx>
#include <TDataStd_Name.hxx>
#include <TDataStd_TreeNode.hxx>
#include <TDocStd_Document.hxx>
#include <XCAFDoc.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_DimTolTool.hxx>
#include <XCAFDoc_DocumentTool.hxx>
#include <XCAFDoc_LayerTool.hxx>
#include <XCAFDoc_MaterialTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>
#include <filesystem>
#include <set>
#include <vector>

namespace occutils::xde {

Doc::Doc() : Standard_Transient() { this->NewDocument(); }

//-----------------------------------------------------------------------------

Doc::Doc(const Handle(TDocStd_Document) & doc) : Standard_Transient() {
  this->init(doc);
}

// -----------------------------------------------------------------------------

void Doc::NewDocument() {
  this->init(this->newDocument());  // Initialize internal structure.
}

// -----------------------------------------------------------------------------

bool Doc::LoadSTEP(const std::string& filename) {
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

  if (!reader.Transfer(m_doc)) {
    return false;
  }

  return true;
}

// -----------------------------------------------------------------------------

bool Doc::SaveSTEP(const std::string& filename, const std::string& units) {
  // Check input units.
  if (units != "MM"       //
      && units != "INCH"  //
      && units != "FT"    //
      && units != "MI"    //
      && units != "M"     //
      && units != "KM"    //
      && units != "MIL"   //
      && units != "UM"    //
      && units != "CM"    //
      && units != "UI") {
    std::cerr << "Wrong units name: " << units << std::endl;
    return false;
  }

  STEPControl_Controller::Init();

  // Set output units.
  Interface_Static::SetCVal("write.step.unit", units.c_str());

  // Do not write pcurves.
  Interface_Static::SetIVal("write.surfacecurve.mode", 0);

  try {
    STEPCAFControl_Writer writer;
    {
      // Save information
      Handle(StepData_StepModel) stepModel = writer.ChangeWriter().Model();
      if (stepModel.IsNull()) return false;

      APIHeaderSection_MakeHeader headerMaker(stepModel);

      Handle(TCollection_HAsciiString) author =
          new TCollection_HAsciiString("occutils");
      Handle(TCollection_HAsciiString) originatingSystem =
          new TCollection_HAsciiString("occutils");
      Handle(TCollection_HAsciiString) organization =
          new TCollection_HAsciiString("occutils");

      headerMaker.SetAuthorValue(1, author);
      headerMaker.SetOriginatingSystem(originatingSystem);
      headerMaker.SetOrganizationValue(1, organization);

      STEPControl_StepModelType mode = STEPControl_AsIs;
      switch (Interface_Static::IVal("write.step.mode")) {
        case 0:
          mode = STEPControl_AsIs;
          break;
        case 1:
          mode = STEPControl_FacetedBrep;
          break;
        case 2:
          mode = STEPControl_ShellBasedSurfaceModel;
          break;
        case 3:
          mode = STEPControl_ManifoldSolidBrep;
          break;
        case 4:
          mode = STEPControl_GeometricCurveSet;
          break;
        default:
          break;
      }

      Standard_CString multiFile = nullptr;
      int extMode = Interface_Static::IVal("write.step.extern.mode");
      //
      if (extMode != 0) {
        // get prefix for file
        multiFile = Interface_Static::CVal("write.step.extern.prefix");
      }

      // Disable writing of GDT if not AP242
      int ap = Interface_Static::IVal("write.step.schema");
      if (ap != 5) writer.SetDimTolMode(false);

      if (!writer.Transfer(this->m_doc, mode, multiFile)) {
        std::cerr << "STEP writer failed (error while transferring "
                     "document into STEP model)."
                  << std::endl;
        return false;
      }
    }
    {
      std::cout << "Flush model into file" << std::endl;

      // Parse directory from filename
      std::filesystem::path filePath(filename);

      // Check if directory exists
      if (std::filesystem::path dirPath = filePath.parent_path();
          !std::filesystem::exists(dirPath) &&
          !std::filesystem::create_directories(dirPath)) {
        // Handle error: unable to create directory
        return false;
      }

      if (writer.Write(filename.c_str()) != IFSelect_RetDone) {
        std::cerr << "STEP writer failed (error while flushing produced model "
                     "into file)."
                  << std::endl;
        return false;
      }
    }
    return true;
  } catch (...) {
    std::cerr << "STEP writer failed (exception occurred)." << std::endl;
    return false;
  }
}

// -----------------------------------------------------------------------------

bool Doc::IsEmpty() const {
  if (m_doc.IsNull()) return true;

  TDF_ChildIterator cit(m_doc->Main());
  const bool isDocEmpty = !cit.More();

  return isDocEmpty;
}

// -----------------------------------------------------------------------------

TDF_Label Doc::AddShape(const TopoDS_Shape& shape,
                        const std::string& shapeName) const {
  TDF_Label shapeLabel = this->GetShapeTool()->NewShape();
  this->GetShapeTool()->SetShape(shapeLabel, shape);

  if (!shapeName.empty()) {
    TDataStd_Name::Set(shapeLabel, shapeName.c_str());
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label Doc::AddShapeWithProps(const TopoDS_Shape& shape,
                                 const ShapeProperties& props) {
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
    this->GetMaterialTool()->SetMaterial(shapeLabel, materialLabel);
  }

  return shapeLabel;
}

// -----------------------------------------------------------------------------

TDF_Label Doc::GetLabel(const TopoDS_Shape& shape) const {
  if (shape.IsNull()) return {};

  TDF_Label foundLabel;
  TDF_ChildIterator it(this->GetShapeTool()->Label(), Standard_True);
  for (; it.More(); it.Next()) {
    TDF_Label currentLabel = it.Value();
    TopoDS_Shape currentShape = XCAFDoc_ShapeTool::GetShape(currentLabel);
    if (currentShape.IsSame(shape)) {
      foundLabel = currentLabel;
      break;
    }
  }
  return foundLabel;
}

// -----------------------------------------------------------------------------

TDF_Label Doc::FindOrCreateMaterial(const Material& material) {
  Handle(XCAFDoc_MaterialTool) MT = this->GetMaterialTool();

  // Check if the material already exists
  TDF_LabelSequence materials;
  MT->GetMaterialLabels(materials);
  for (Standard_Integer i = 1; i <= materials.Length(); i++) {
    TDF_Label materialLabel = materials.Value(i);
    Handle(TCollection_HAsciiString) name;
    Handle(TCollection_HAsciiString) description;
    Standard_Real density;
    Handle(TCollection_HAsciiString) densName;
    Handle(TCollection_HAsciiString) densValType;

    if (XCAFDoc_MaterialTool::GetMaterial(materialLabel, name, description,
                                          density, densName, densValType)) {
      Material extMaterial(name->ToCString(), description->ToCString(), density,
                           densName->ToCString(), densValType->ToCString());
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

  TDF_Label newMaterialLabel = MT->AddMaterial(
      name, description, material.GetDensity(), densName, densValType);

  return newMaterialLabel;
}

// -----------------------------------------------------------------------------

std::vector<TDF_Label> Doc::GetMaterials() const {
  TDF_LabelSequence materials;
  this->GetMaterialTool()->GetMaterialLabels(materials);

  std::vector<TDF_Label> materialLabels;
  for (Standard_Integer i = 1; i <= materials.Length(); i++) {
    materialLabels.push_back(materials.Value(i));
  }
  return materialLabels;
}

//-----------------------------------------------------------------------------

TopoDS_Shape Doc::GetShape(const TDF_Label& label) {
  if (label.IsNull()) return {};

  return XCAFDoc_ShapeTool::GetShape(label);
}

//-----------------------------------------------------------------------------

TopoDS_Shape Doc::GetOneShape() const {
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

bool Doc::GetColor(const TDF_Label& label, Quantity_Color& color) const {
  Quantity_ColorRGBA colorRGBA;
  const bool isOk = this->GetColor(label, colorRGBA);

  if (isOk) color = colorRGBA.GetRGB();

  return isOk;
}

//-----------------------------------------------------------------------------

bool Doc::GetColor(const TDF_Label& label, Quantity_ColorRGBA& color) const {
  bool isColorFound = false;

  if (!this->GetColorTool().IsNull()) {
    // Get the source label.
    TDF_Label refLabel = label;
    //
    if (XCAFDoc_ShapeTool::IsReference(label))
      XCAFDoc_ShapeTool::GetReferredShape(label, refLabel);

    // Get one of the possibly available colors.
    isColorFound =
        XCAFDoc_ColorTool::GetColor(refLabel, XCAFDoc_ColorSurf, color);

    if (!isColorFound)
      isColorFound =
          XCAFDoc_ColorTool::GetColor(refLabel, XCAFDoc_ColorGen, color);

    if (!isColorFound)
      isColorFound =
          XCAFDoc_ColorTool::GetColor(refLabel, XCAFDoc_ColorCurv, color);
  }

  return isColorFound;
}

//-----------------------------------------------------------------------------

bool Doc::GetColorAlpha(const TDF_Label& label, double& alpha) const {
  // Get color for the part itself.
  Quantity_ColorRGBA colorRGBA;
  bool isColorFound = this->GetColor(label, colorRGBA);

  if (isColorFound) {
    alpha = colorRGBA.Alpha();
  } else {  // Try getting color from any of the sub-shapes as a fallback
            // solution
    TDF_LabelSequence subShapes;
    XCAFDoc_ShapeTool::GetSubShapes(label, subShapes);

    if (subShapes.IsEmpty()) return isColorFound;

    if (TDF_LabelSequence::Iterator it(subShapes); it.More())
      isColorFound = this->GetColor(it.Value(), colorRGBA);

    if (isColorFound) alpha = colorRGBA.Alpha();
  }

  return isColorFound;
}

//-----------------------------------------------------------------------------

void Doc::SetColor(const TDF_Label& label, const Quantity_Color& color) {
  if (label.IsNull()) return;

  TCollection_AsciiString partId;
  TDF_Tool::Entry(label, partId);

  Quantity_ColorRGBA colorRGBA(color);

  if (Quantity_ColorRGBA oldColorRGBA; this->GetColor(label, oldColorRGBA))
    colorRGBA.SetAlpha(oldColorRGBA.Alpha());

  this->SetColor(label, colorRGBA, true);
}

//-----------------------------------------------------------------------------

void Doc::SetColor(const TDF_Label& label, const Quantity_ColorRGBA& color,
                   const bool changeTransp) {
  if (label.IsNull()) return;

  /*
   * Colors should be set to non-assemblies labels only. Assigning color to
   * instances or subassemblies is allowed, but we avoid it to simplify the data
   * architecture.
   */

  if (XCAFDoc_ShapeTool::IsReference(label)) {
    // Set color to the prototype.
    TDF_Label protoLab;
    XCAFDoc_ShapeTool::GetReferredShape(label, protoLab);

    this->SetColor(protoLab, color, changeTransp);
  } else if (XCAFDoc_ShapeTool::IsAssembly(label)) {
    // Set color to all assembly components.
    TDF_LabelSequence components;
    XCAFDoc_ShapeTool::GetComponents(label, components, true);

    for (TDF_LabelSequence::Iterator cit(components); cit.More(); cit.Next()) {
      this->SetColor(cit.Value(), color, changeTransp);
    }
  } else /* Shape */
  {
    TopoDS_Shape shape = XCAFDoc_ShapeTool::GetShape(label);

    if (shape.IsNull()) return;

    Quantity_ColorRGBA colorRGBA(color);

    if (!changeTransp) {
      double alpha = 1.0;

      if (this->GetColorAlpha(label, alpha)) colorRGBA.SetAlpha((float)alpha);
    }

    Handle(XCAFDoc_ColorTool) CT = this->GetColorTool();

    Handle(TDataStd_TreeNode) colorAttr;
    bool isGenColor =
        label.FindAttribute(XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), colorAttr);

    if (shape.ShapeType() == TopAbs_EDGE) {
      CT->SetColor(label, colorRGBA, XCAFDoc_ColorCurv);
    } else {
      CT->SetColor(label, colorRGBA, XCAFDoc_ColorSurf);
      CT->SetColor(label, colorRGBA, XCAFDoc_ColorCurv);
    }
    if (isGenColor) {
      CT->SetColor(label, colorRGBA, XCAFDoc_ColorGen);
    }

    TDF_LabelSequence subshapes;
    XCAFDoc_ShapeTool::GetSubShapes(label, subshapes);

    for (TDF_LabelSequence::Iterator iter(subshapes); iter.More();
         iter.Next()) {
      if (this->GetShape(iter.Value()).ShapeType() == TopAbs_EDGE) {
        CT->SetColor(iter.Value(), colorRGBA, XCAFDoc_ColorCurv);
      } else {
        CT->SetColor(iter.Value(), colorRGBA, XCAFDoc_ColorSurf);
        CT->SetColor(iter.Value(), colorRGBA, XCAFDoc_ColorCurv);
      }

      isGenColor = iter.Value().FindAttribute(
          XCAFDoc::ColorRefGUID(XCAFDoc_ColorGen), colorAttr);
      if (isGenColor) {
        CT->SetColor(iter.Value(), colorRGBA, XCAFDoc_ColorGen);
      }
    }
  }
}

// -----------------------------------------------------------------------------

void Doc::ResetColors() const {
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

Handle(TDocStd_Document) & Doc::ChangeDocument() { return m_doc; }

// -----------------------------------------------------------------------------

const Handle(TDocStd_Document) & Doc::GetDocument() const { return m_doc; }

// -----------------------------------------------------------------------------

Handle(XCAFDoc_ShapeTool) Doc::GetShapeTool() const {
  return XCAFDoc_DocumentTool::ShapeTool(m_doc->Main());
}

// -----------------------------------------------------------------------------

Handle(XCAFDoc_ColorTool) Doc::GetColorTool() const {
  return XCAFDoc_DocumentTool::ColorTool(m_doc->Main());
}

// -----------------------------------------------------------------------------

Handle(XCAFDoc_MaterialTool) Doc::GetMaterialTool() const {
  return XCAFDoc_DocumentTool::MaterialTool(m_doc->Main());
}

// -----------------------------------------------------------------------------

void Doc::init(const Handle(TDocStd_Document) & doc) {
  // Store the pointer to the passed Document in the member field.
  m_doc = doc;
}

//-----------------------------------------------------------------------------

Handle(TDocStd_Document) Doc::newDocument() {
  Handle(TDocStd_Document) D;
  Handle(App) A = this->getApplication();

  // Create XDE Document and return.
  A->NewDocument("BinXCAF", D);
  return D;
}

//-----------------------------------------------------------------------------

Handle(App) Doc::getApplication() { return App::Instance(); }

}  // namespace occutils::xde
