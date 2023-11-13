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
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorType.hxx>

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
                     const std::string& shapeName = std::string());

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
