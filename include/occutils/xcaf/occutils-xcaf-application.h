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

#pragma once

#include <Quantity_Color.hxx>
#include <TDF_Label.hxx>
#include <TDocStd_Document.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorTool.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <XCAFDoc_ShapeTool.hxx>

#include "occutils/xcaf/occutils-xcaf-shape.h"

namespace occutils::xcaf {

/**
 * @struct ExtendedXCAFApplicationInternals
 * @brief Contains internal state and tools for the ExtendedXCAFApplication.
 *
 * ExtendedXCAFApplicationInternals initializes document and tools required
 * for STEP exporting and contains storage for shape labels. Handles
 * operations related to document creation, tool initialization,
 * and internal storage management for label associations with shapes.
 */
struct ExtendedXCAFApplicationInternals;

/**
 * @class ExtendedXCAFApplication
 * @brief Manages the export of shapes with extended STEP attributes.
 *
 * ExtendedXCAFApplication provides functionalities to manage and export
 * 3D shapes and their associated attributes (e.g., colors, materials)
 * in the STEP file format. It allows clients to add shapes with or
 * without specific attributes and to write the entire data to a STEP file.
 *
 * Usage example:
 * @code
 * ExtendedXCAFApplication app;
 * app.AddShape(shape);
 * app.WriteSTEP("path_to_export.step");
 * @endcode
 *
 * @note This class utilizes a Pimpl idiom to hide implementation details
 *       and dependencies in the header file, aiding in reducing
 *       compilation times and providing ABI stability.
 */
class ExtendedXCAFApplication {
 public:
  /**
   * @brief Constructs a new ExtendedXCAFApplication object.
   *
   * Initializes internal state and prepares tools for exporting.
   */
  ExtendedXCAFApplication();

  /**
   * @brief Destructs the ExtendedXCAFApplication object.
   *
   * Ensures safe cleanup, including closing open documents.
   */
  ~ExtendedXCAFApplication();

  /**
   * @brief Adds a shape without special attributes to the application.
   *
   * @param shape The shape to be added to the application.
   * @param shapeName The name to be associated with the shape.
   *
   * @return TDF_Label The label of the added shape in the internal storage.
   *         Can be used for subsequent referencing.
   */
  TDF_Label AddShape(const TopoDS_Shape& shape,
                     const std::string& shapeName = std::string()) const;

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
  TDF_Label AddShapeWithProps(const TopoDS_Shape& shape,
                              const XCAFShapeProperties& props);

  /**
   * @brief Finds an existing material in the XCAF document or creates a new
   * one.
   *
   * This method searches for a material in the XCAF document that matches the
   * properties of the given XCAFMaterial object. If such a material is found,
   * its label is returned. Otherwise, a new material is created with the
   * provided properties, and the label of this new material is returned.
   *
   * @param material The XCAFMaterial object containing the properties of the
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
   * match with those of the provided XCAFMaterial object, it is considered a
   * match.
   *
   * @example
   *      using occutils::xcaf;
   *
   *      XCAFMaterial material("Steel", "High-grade steel", 7.85, "kg/m^3",
   *        "Density");
   *      TDF_Label materialLabel = app.FindOrCreateMaterial(material);
   *      // Use materialLabel as needed
   */
  TDF_Label FindOrCreateMaterial(const XCAFMaterial& material);

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
   * @brief Removes all color attributes from the shapes in the current
   * document.
   *
   * This method iterates through all the shapes in the document and removes any
   * color attributes assigned to them. It effectively resets the color state of
   * the entire document, leaving shapes without any color properties.
   *
   * Usage:
   *     ExtendedXCAFApplication app;
   *     // ... document setup and operations ...
   *     app.ResetColors(); // Resets colors for all shapes in the document
   *
   * @note This operation might be irreversible for the current session if the
   *       document's state is not saved beforehand. It's recommended to save
   *       the document or maintain a backup before performing this operation
   *       if the original color data is important.
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
  Handle(TDocStd_Document) & ChangeDocument();

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
  [[nodiscard]] const Handle(TDocStd_Document) & GetDocument() const;

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
   * @brief Reads shapes from a STEP file.
   *
   * @param filename The path and name of the file to read from.
   * @return true if successful, false otherwise.
   */
  bool ReadSTEP(const std::string& filename);

  /**
   * @brief Exports shapes and their attributes to a STEP file.
   *
   * Writes all shapes and their associated attributes that have been
   * added to the exporter to the specified STEP file.
   *
   * @param filename The path and name of the file to write to.
   * @param exportUnit The unit to be used for exporting. Defaults to "MM".
   */
  bool WriteSTEP(const std::string& filename,
                 const std::string& exportUnit = "MM");

 private:
  std::unique_ptr<ExtendedXCAFApplicationInternals> internals;
};

}  // namespace occutils::xcaf
