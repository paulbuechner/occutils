/***************************************************************************
 *   Created on: 15 Oct 2023                                               *
 ***************************************************************************
 *   Copyright (c) 2023-present, Paul Buechner                             *
 *                                                                         *
 *   This file is part of the occutils library.                            *
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

#pragma once

// std includes
#include <functional>
#include <vector>

// OCC includes
#include <APIHeaderSection_MakeHeader.hxx>
#include <Quantity_Color.hxx>
#include <Standard_Type.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_MaterialTool.hxx>
#include <XCAFDoc_ShapeTool.hxx>

// occutils xde includes
#include "occutils/xde/occutils-xde-app.h"
#include "occutils/xde/occutils-xde-material.h"
#include "occutils/xde/occutils-xde-shape.h"

namespace occutils::xde
{

/**
 * @struct DocInternals
 * @brief Contains internal state and tools for the Doc.
 *
 * DocInternals initializes document and tools required
 * for STEP exporting and contains storage for shape labels. Handles
 * operations related to document creation, tool initialization,
 * and internal storage management for label associations with shapes.
 */
struct DocInternals;

/**
 * @class Doc
 * @brief Manages the export of shapes with extended STEP attributes.
 *
 * Doc provides functionalities to manage and export
 * 3D shapes and their associated attributes (e.g., colors, materials)
 * in the STEP file format. It allows clients to add shapes with or
 * without specific attributes and to write the entire data to a STEP file.
 *
 * Usage example:
 * @code
 * Doc app;
 * app.AddShape(shape);
 * app.WriteSTEP("path_to_export.step");
 * @endcode
 *
 * @note This class utilizes a Pimpl idiom to hide implementation details
 *       and dependencies in the header file, aiding in reducing
 *       compilation times and providing ABI stability.
 */
class Doc : public Standard_Transient
{
  // OCCT RTTI
  DEFINE_STANDARD_RTTI_INLINE(Doc, Standard_Transient)

public:
  /**
   * @brief Constructs a new Doc object.
   *
   * Initializes internal state and prepares tools for exporting.
   */
  Doc();

  /**
   * @brief Constructs a new Doc object with the provided CAF Document.
   *
   * @param doc The CAF Document to initialize the Doc with.
   */
  Doc(const Handle(TDocStd_Document)& doc);

  /*
   * Construction and Initialization
   */
public:
  /**
   * @brief Creates new empty XDE Document under this Assembly Document facade.
   */
  void NewDocument();

  /**
   * @brief Loads shapes from a STEP file.
   *
   * @param filename The path and name of the file to read from.
   *
   * @return true if successful, false otherwise.
   */
  bool LoadSTEP(const std::string& filename);

  /**
   * @brief Exports shapes and their attributes to a STEP file.
   *
   * Writes all shapes and their associated attributes that have been
   * added to the exporter to the specified STEP file.
   *
   * @param filename The path and name of the file to write to.
   * @param exportUnit The unit to be used for exporting. Defaults to "MM".
   * @param headerCustomizer A Function to customize the header section (optional).
   *
   * @return true if successful, false otherwise.
   */
  bool SaveSTEP(const std::string&                                filename,
                const std::string&                                exportUnit       = "MM",
                std::function<void(APIHeaderSection_MakeHeader&)> headerCustomizer = {}) const;

  /**
   * @brief Checks if the Assembly Document is empty.
   *
   * @return true if the Assembly Document is empty, i.e. the XDE Document is
   * either null or contains no roots.
   */
  bool IsEmpty() const;

  /*
   * API
   */
public:
  /**
   * @brief Adds a shape without special attributes to the application.
   *
   * @param shape The shape to be added to the application.
   * @param shapeName The name to be associated with the shape.
   *
   * @return TDF_Label The label of the added shape in the internal storage.
   *         Can be used for subsequent referencing.
   */
  TDF_Label AddShape(const TopoDS_Shape& shape, const std::string& shapeName = std::string()) const;

  /**
   * @brief Adds a shape with specified properties to the application.
   *
   * @param shape The shape to be added to the application.
   * @param props The properties (color, name, transparency, material) to be
   * associated with the shape.
   *
   * @return TDF_Label The label of the added shape in the internal storage.
   *         Can be used for subsequent referencing.
   */
  TDF_Label AddShapeWithProps(const TopoDS_Shape& shape, const ShapeProperties& props);

  /**
   * @brief Retrieves the TDF_Label associated with a given TopoDS_Shape in the
   * document.
   *
   * This method iterates through the child labels of the document's shape tool
   * and compares each shape with the provided TopoDS_Shape. The comparison uses
   * the IsSame method, which checks for topological equivalence.
   *
   * @param shape The shape for which the TDF_Label is to be found. If the shape
   * is null (i.e., IsNull() returns true), the method returns an empty
   * TDF_Label.
   *
   * @return The label corresponding to the provided shape. If the shape is not
   * found in the document, an empty TDF_Label is returned. An empty TDF_Label
   * can be checked using its IsNull() method.
   *
   * @note This method can be computationally intensive for documents with a
   * large number of shapes, as it involves iterating through all child labels
   * in the document. It is recommended to use this method only when necessary.
   */
  [[nodiscard]] TDF_Label GetLabel(const TopoDS_Shape& shape) const;

  /**
   * @brief Finds an existing material in the XCAF document or creates a new
   * one.
   *
   * This method searches for a material in the XCAF document that matches the
   * properties of the given Material object. If such a material is found, its
   * label is returned. Otherwise, a new material is created with the provided
   * properties, and the label of this new material is returned.
   *
   * @param material The Material object containing the properties of the
   * material to be found or created. This includes the material's name,
   *                 description, density, density name, and density value type.
   *
   * @return TDF_Label The label of the found or created material in the XCAF
   * document. If an existing material is found that matches the given
   * properties, the label of this material is returned. Otherwise, a new
   * material is created, and its label is returned.
   *
   * @note The comparison of materials is based on the equality of their
   * properties. If all properties of an existing material in the XCAF document
   * match with those of the provided Material object, it is considered a
   * match.
   *
   * @example
   *      using occutils::xcaf;
   *
   *      Material material("Steel", "High-grade steel", 7.85, "kg/m^3",
   *        "Density");
   *      TDF_Label materialLabel = app.FindOrCreateMaterial(material);
   *      // Use materialLabel as needed
   */
  TDF_Label FindOrCreateMaterial(const Material& material) const;

  /**
   * @brief Retrieves the labels of all materials in the application.
   *
   * @return The labels of all materials in the
   */
  [[nodiscard]] std::vector<TDF_Label> GetMaterials() const;

  /**
   * @brief Retrieves the TopoDS_Shape associated with a given label in the
   * document.
   *
   * This method fetches the geometric shape associated with the provided label.
   * It is useful for retrieving specific shapes based on their labels within
   * the document.
   *
   * @param label The label of the shape to retrieve. This label should be
   * valid and associated with a shape in the document.
   *
   * @return The geometric shape associated with the given label. Returns an
   * empty shape if the label is null or does not correspond to a valid shape.
   */
  [[nodiscard]] static TopoDS_Shape GetShape(const TDF_Label& label);

  /**
   * @brief Retrieves a single shape or a compound of all free shapes in the
   * document.
   *
   * This method is designed to handle documents with varying numbers of shapes.
   * If the document contains exactly one free shape, that shape is returned.
   * If the document contains multiple shapes, they are combined into a single
   * compound shape and returned.
   *
   * @return TopoDS_Shape A single shape or a compound of all free shapes in the
   * document. Returns an empty shape if the document contains no free shapes.
   */
  [[nodiscard]] TopoDS_Shape GetOneShape() const;

  /**
   * @brief Retrieves the color of the given label as a Quantity_Color object.
   *
   * @param label The label from which the color needs to be retrieved.
   * @param color Reference to a Quantity_Color object to store the retrieved
   * color.
   *
   * @return true if the color is successfully retrieved and false otherwise.
   */
  bool GetColor(const TDF_Label& label, Quantity_Color& color) const;

  /**
   * @brief Retrieves the RGBA color of the given label.
   *
   * This method checks if a color is associated with the given label. If the
   * label is a reference, it retrieves the color of the referred shape. It
   * tries to get the surface, generic, and curve colors in that order.
   *
   * @param label The TDF_Label from which the color needs to be retrieved.
   * @param color Reference to a Quantity_ColorRGBA object to store the
   * retrieved color.
   *
   * @return true if any color is found, false otherwise.
   */
  bool GetColor(const TDF_Label& label, Quantity_ColorRGBA& color) const;

  /**
   * @brief Retrieves the alpha (transparency) value of the color associated
   * with the given label.
   *
   * This method first tries to get the color and its alpha for the given label.
   * If no color is found, it checks the sub-shapes and retrieves the alpha
   * value from the first sub-shape with a color.
   *
   * @param label The label from which the alpha value needs to be retrieved.
   * @param alpha Reference to a double to store the retrieved alpha value.
   *
   * @return true if the alpha value is successfully retrieved, false otherwise.
   */
  bool GetColorAlpha(const TDF_Label& label, double& alpha) const;

  /**
   * @brief Sets the color of a given label in RGB format.
   *
   * This method sets the color of the provided label. If the label already has
   * a color with an alpha value, that alpha value is preserved. The method
   * handles different cases like prototypes, assemblies, and individual shapes
   * differently.
   *
   * @param label The label for which the color is to be set.
   * @param color The RGB color to be set.
   */
  void SetColor(const TDF_Label& label, const Quantity_Color& color);

  /**
   * @brief Sets the color of a given label in RGBA format.
   *
   * This method sets the color of the provided label, including its
   * transparency (alpha) if specified. It deals with references, assemblies,
   * and individual shapes separately. The method sets both surface and curve
   * colors, and general color if applicable.
   *
   * @param label The TDF_Label for which the color is to be set.
   * @param color The RGBA color to be set.
   * @param changeTransp A boolean flag to indicate whether to change the
   * transparency.
   */
  void SetColor(const TDF_Label& label, const Quantity_ColorRGBA& color, bool changeTransp);

  /**
   * @brief Removes all color attributes from the shapes in the current
   * document.
   *
   * This method iterates through all the shapes in the document and removes
   * any color attributes assigned to them. It effectively resets the color
   * state of the entire document, leaving shapes without any color
   * properties.
   *
   * Usage:
   *     Doc app;
   *     // ... document setup and operations ...
   *     app.ResetColors(); // Resets colors for all shapes in the document
   *
   * @note This operation might be irreversible for the current session if
   * the document's state is not saved beforehand. It's recommended to save
   *       the document or maintain a backup before performing this
   * operation if the original color data is important.
   */
  void ResetColors() const;

  /**
   * @brief Retrieves a modifiable reference to the current document.
   *
   * This method provides access to the current document with the ability to
   * modify it. It is useful for operations that need to change the document's
   * state or its contents.
   *
   * @return A reference to the handle of the current document. The handle
   * allows both reading and modification of the document.
   */
  Handle(TDocStd_Document)& ChangeDocument();

  /**
   * @brief Retrieves a constant reference to the current document.
   *
   * This method provides read-only access to the current document. It is used
   * for operations that require inspecting the document without modifying it.
   *
   * @return A constant reference to the handle of the current document. This
   * handle is read-only.
   *
   * @note Marked as [[nodiscard]] to encourage checking the returned handle, as
   * it provides essential document access.
   */
  [[nodiscard]] const Handle(TDocStd_Document)& GetDocument() const;

  /**
   * @brief Retrieves the shape tool associated with the current document.
   *
   * This method provides access to the XCAFDoc_ShapeTool, which is used for
   * managing and manipulating shapes within the document. The shape tool is
   * central to operations involving geometric shapes.
   *
   * @return A handle to the shape tool of the current document.
   */
  Handle(XCAFDoc_ShapeTool) GetShapeTool() const;

  /**
   * @brief Retrieves the color tool associated with the current document.
   *
   * This method provides access to the XCAFDoc_ColorTool, which is responsible
   * for managing color attributes of the shapes in the document. It allows for
   * the application, modification, and querying of color properties.
   *
   * @return A handle to the color tool of the current document.
   */
  Handle(XCAFDoc_ColorTool) GetColorTool() const;

  /**
   * @brief Retrieves the material tool associated with the current document.
   *
   * This method provides access to the XCAFDoc_MaterialTool, which is
   * responsible for managing material attributes of the shapes in the
   * document. It allows for the application, modification, and querying of
   * material properties.
   *
   * @return A handle to the material tool of the current document.
   */
  Handle(XCAFDoc_MaterialTool) GetMaterialTool() const;

protected:
  /**
   * @brief Initializes the Data Model with the passed CAF Document and prepares
   * integral Data Model Engines.
   *
   * @param doc The CAF Document to initialize the Model with.
   */
  void init(const Handle(TDocStd_Document)& doc);

  /**
   * @brief Creates a new CAF Document.
   *
   * @return The newly created CAF Document.
   */
  Handle(TDocStd_Document) newDocument();

  /**
   * @brief Retrieves the XDE Application instance.
   *
   * @return The XDE Application instance.
   */
  Handle(App) getApplication();

protected:
  Handle(TDocStd_Document) m_doc; //!< Underlying XCAF document.
};

} // namespace occutils::xde
