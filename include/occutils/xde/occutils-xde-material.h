#pragma once

// std includes
#include <memory>
#include <string>
#include <utility>

// OCC includes
#include <Quantity_Color.hxx>
#include <TDF_Label.hxx>
#include <TopoDS_Shape.hxx>
#include <XCAFDoc_ColorType.hxx>

namespace occutils::xde {

/**
 * @brief Represents material properties in the application.
 *
 * This class encapsulates various attributes of a material, such as its name,
 * description, density, density name, and density value type. It provides
 * methods to set and retrieve these properties, allowing for a structured and
 * convenient handling of material data in the application.
 */
class Material {
 public:
  /**
   * @brief Constructor for Material with optional parameters for
   * initialization.
   *
   * @param name The name of the material.
   * @param description A brief description of the material.
   * @param density The density of the material.
   * @param densName The name associated with the density measurement.
   * @param densValType The type of the density value (e.g., kg/m^3).
   */
  explicit Material(std::string name = "", std::string description = "",
                    double density = 0.0, std::string densName = "",
                    std::string densValType = "")
      : m_name(std::move(name)),
        m_description(std::move(description)),
        m_density(density),
        m_densName(std::move(densName)),
        m_densValType(std::move(densValType)) {}

  /**
   * @brief Checks if two Material instances are equal.
   *
   * @param other The Material instance to compare with.
   * @return true if all properties of both instances are equal.
   * @return false otherwise.
   */
  bool operator==(const Material& other) const {
    return m_name == other.m_name && m_description == other.m_description &&
           m_density == other.m_density && m_densName == other.m_densName &&
           m_densValType == other.m_densValType;
  }

  /**
   * @brief Sets the name of the material.
   *
   * @param name The name to be assigned to the material.
   */
  void SetName(const std::string_view name) { m_name = name; }

  /**
   * @brief Sets the description of the material.
   *
   * @param description The description to be assigned to the material.
   */
  void SetDescription(const std::string_view description) {
    m_description = description;
  }

  /**
   * @brief Sets the density of the material.
   *
   * @param density The density to be assigned to the material.
   */
  void SetDensity(double density) { m_density = density; }

  /**
   * @brief Sets the name associated with the density of the material.
   *
   * @param densName The density name to be assigned to the material.
   */
  void SetDensName(const std::string_view densName) { m_densName = densName; }

  /**
   * @brief Sets the type of the density value of the material.
   *
   * @param densValType The density value type to be assigned to the material.
   */
  void SetDensValType(const std::string_view densValType) {
    m_densValType = densValType;
  }

  /**
   * @brief Retrieves the name of the material.
   *
   * @return const std::string& The name of the material.
   */
  [[nodiscard]] const std::string& GetName() const { return m_name; }

  /**
   * @brief Retrieves the description of the material.
   *
   * @return const std::string& The description of the material.
   */
  [[nodiscard]] const std::string& GetDescription() const {
    return m_description;
  }

  /**
   * @brief Retrieves the density of the material.
   *
   * @return double The density of the material.
   */
  [[nodiscard]] double GetDensity() const { return m_density; }

  /**
   * @brief Retrieves the name associated with the density of the material.
   *
   * @return const std::string& The name associated with the material's density.
   */
  [[nodiscard]] const std::string& GetDensityName() const { return m_densName; }

  /**
   * @brief Retrieves the type of the density value of the material.
   *
   * @return const std::string& The type of the density value of the material.
   */
  [[nodiscard]] const std::string& GetDensityValueType() const {
    return m_densValType;
  }

  /**
   * @brief Checks if the material is null or empty.
   *
   * This method determines if all the properties of the material are in their
   * default state, which typically means the material is considered null or
   * uninitialized.
   *
   * @return true if the material is null/empty.
   * @return false otherwise.
   */
  [[nodiscard]] bool IsNull() const {
    return m_name.empty() && m_description.empty() && m_density == 0.0 &&
           m_densName.empty() && m_densValType.empty();
  }

 private:
  /**
   * @brief Name of the material.
   */
  std::string m_name;

  /**
   * @brief Description of the material.
   */
  std::string m_description;

  /**
   * @brief Density of the material.
   */
  double m_density;

  /**
   * @brief Name associated with the density.
   */
  std::string m_densName;

  /**
   * @brief Type of the density value.
   */
  std::string m_densValType;
};

}  // namespace occutils::xde
