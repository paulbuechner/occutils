#pragma once

#include <Quantity_Color.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorType.hxx>
#include <memory>

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
   * @return size_t Index of the shape in the internal storage.
   *         Can be used for subsequent referencing.
   */
  size_t AddShape(const TopoDS_Shape& shape,
                  const std::string& shapeName = std::string());

  /**
   * @brief Adds a shape with associated color to the application.
   *
   * @param shape The shape to be added to the application.
   * @param color The color to be associated with the shape.
   * @param shapeName The name to be associated with the shape.
   * @param colorType The type of the color. Defaults to XCAFDoc_ColorGen.
   *
   * @return size_t Index of the shape in the internal storage.
   */
  size_t AddShapeWithColor(const TopoDS_Shape& shape,
                           const Quantity_Color& color,
                           const std::string& shapeName = std::string(),
                           XCAFDoc_ColorType colorType = XCAFDoc_ColorGen);

  /**
   * @brief Exports shapes and their attributes to a STEP file.
   *
   * Writes all shapes and their associated attributes that have been
   * added to the exporter to the specified STEP file.
   *
   * @param filename The path and name of the file to write to.
   * @param exportUnit The unit to be used for exporting. Defaults to "MM".
   */
  void WriteSTEP(const std::string& filename,
                 const std::string& exportUnit = "MM");

 private:
  std::unique_ptr<ExtendedXCAFApplicationInternals> internals;
};

}  // namespace occutils::xcaf
