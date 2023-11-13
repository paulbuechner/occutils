#pragma once

#include <Quantity_Color.hxx>
#include <Quantity_ColorRGBA.hxx>
#include <TDF_Label.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorType.hxx>

#include "occutils-xcaf-material.h"

namespace occutils::xcaf {

/**
 * @brief Represents properties for a shape to be added to an application.
 *
 * This class encapsulates various attributes like color, name and material for
 * a shape. It provides setters and getters for these attributes.
 */
class XCAFShapeProperties {
 public:
  // Default constructor
  XCAFShapeProperties() = default;

  /**
   * @brief Set the Name of the shape.
   *
   * @param name The name to be associated with the shape.
   */
  void SetName(const std::string_view name) { m_name = name; }

  /**
   * @brief Set the Color of the shape.
   *
   * @param color The color to be associated with the shape.
   * @param colorType The type of the color. Defaults to XCAFDoc_ColorGen.
   */
  void SetColor(const Quantity_Color& color,
                XCAFDoc_ColorType colorType = XCAFDoc_ColorGen) {
    m_color = Quantity_ColorRGBA(color);
    m_colorType = colorType;
    m_hasColor = true;
  }

  /**
   * @brief Set the Color of the shape.
   *
   * @param color The color with alpha channel to be associated with the shape.
   * @param colorType The type of the color. Defaults to XCAFDoc_ColorGen.
   */
  void SetColor(const Quantity_ColorRGBA& color,
                XCAFDoc_ColorType colorType = XCAFDoc_ColorGen) {
    m_color = color;
    m_colorType = colorType;
    m_hasColor = true;
  }

  /**
   * @brief Set the Material of the shape.
   *
   * @param material The material to be associated with the shape.
   */
  void SetMaterial(const XCAFMaterial& material) { m_material = material; }

  /**
   * @brief Retrieves the name of the shape.
   *
   * @return const std::string& The name of the shape.
   */
  [[nodiscard]] const std::string& GetName() const { return m_name; }

  /**
   * @brief Retrieves the color of the shape.
   *
   * @return const Quantity_ColorRGBA& The color of the shape.
   */
  [[nodiscard]] const Quantity_ColorRGBA& GetColor() const { return m_color; }

  /**
   * @brief Retrieves the type of color assigned to the shape.
   *
   * @return XCAFDoc_ColorType The type of the color.
   */
  [[nodiscard]] XCAFDoc_ColorType GetColorType() const { return m_colorType; }

  /**
   * @brief Retrieves the material associated with the shape.
   *
   * @return const std::string& The material of the shape.
   */
  [[nodiscard]] const XCAFMaterial& GetMaterial() const { return m_material; }

  /**
   * @brief Checks if a color has been set for the shape.
   *
   * @return bool True if a color has been set, false otherwise.
   */
  [[nodiscard]] bool HasColor() const { return m_hasColor; }

 private:
  /**
   * @brief The name of the shape.
   */
  std::string m_name;

  /**
   * @brief The color of the shape.
   */
  Quantity_ColorRGBA m_color;

  /**
   * @brief The type of the color.
   */
  XCAFDoc_ColorType m_colorType = XCAFDoc_ColorGen;

  /**
   * @brief The material of the shape.
   */
  XCAFMaterial m_material;

  /**
   * @brief Flag to indicate if a color has been set.
   */
  bool m_hasColor = false;
};

}  // namespace occutils::xcaf
