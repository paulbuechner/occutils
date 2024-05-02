#pragma once

// std includes
#include <string>
#include <vector>

// OCC includes
#include <LDOM_Element.hxx>

namespace occutils::ldom {

/**
 * @brief Extracts the local tag name from an XML or HTML element.
 *
 * This function retrieves the tag name of the given element and optionally
 * strips off the namespace prefix. If the tag name contains a colon, it
 * indicates a namespace prefix. The function will return the part after the
 * colon if keepPrefix is false; otherwise, it returns the full tag name.
 *
 * @param el A constant reference to an LDOM_Element object.
 * @param keepPrefix If true, the namespace prefix is retained. If false, only
 *                   the local part of the tag name (after the colon) is
 *                   returned. Defaults to false.
 * @return The local tag name as a std::string.
 */
std::string GetLocalTagName(LDOM_Element const &el, bool keepPrefix = false);

/**
 * @brief Extracts the local attribute name from an XML or HTML node.
 *
 * This function retrieves the node name of the given element and strips off
 * the namespace prefix if present. If the node name contains a colon, it
 * indicates a namespace prefix. The function returns the part of the node name
 * after the colon, which represents the local name of the attribute.
 *
 * @param el A constant reference to an LDOM_Node object representing the
 * XML/HTML node.
 * @return The local attribute name as a std::string, without the namespace
 * prefix.
 */
std::string GetLocalAttrName(LDOM_Node const &el);

/**
 * @brief Retrieves the value of a specified attribute from an element.
 *
 * This function iterates through all attributes of the given LDOM_Element.
 * It compares the name of each attribute (local name, without namespace prefix)
 * to the provided name. If a match is found, it returns the value of that
 * attribute. If no match is found, it returns an empty string.
 *
 * @param el A constant reference to an LDOM_Element object.
 * @param name The name of the attribute to search for, as a std::string_view.
 * @return The value of the attribute as a std::string, or an empty string if
 * the attribute is not found.
 */
std::string GetAttrValue(LDOM_Element const &el, std::string_view name);

/**
 * @brief Retrieves the tag name of the first child element of a given parent
 * element.
 *
 * This function obtains the first child node of the specified parent element.
 * If the first child node is an element node, it retrieves the tag name of this
 * node. The function also allows the option to keep or discard the namespace
 * prefix in the tag name.
 *
 * @param parentEl A constant reference to an LDOM_Element object representing
 * the parent element.
 * @param keepPrefix A boolean flag indicating whether to keep the namespace
 * prefix in the tag name. If true, the full tag name including the prefix is
 * returned; if false, only the local part of the name (after the colon) is
 * returned.
 * @return The tag name of the first child element as a std::string. If the
 * first child is not an element node or no child exists, an empty string is
 * returned.
 */
std::string GetFirstChildTagName(const LDOM_Element &parentEl, bool keepPrefix);

/**
 * @brief Retrieves the all child elements of a given parent element that
 * matches a specified name.
 *
 * This function iterates through the child nodes of the specified parent
 * element \c (parentEl), looking for all element nodes whose tag name matches
 * the \c childName. If such a node is found, it is added to the returned vector
 * of \c LDOM_Element.
 *
 * @param parentEl A constant reference to an \c LDOM_Element object
 * representing the parent element.
 * @param childName A \c std::string_view representing the name of the child
 * elements to search for.
 * @return A vector of \c LDOM_Element objects representing the all matching
 * child elements. If no matching child elements are found, or if \c parentEl is
 * null or \c childName is empty, an empty vector of \c LDOM_Element object is
 * returned.
 */
std::vector<LDOM_Element> GetChildrenByName(const LDOM_Element &parentEl,
                                            std::string_view childName);

/**
 * @brief Retrieves the first child element of a given parent element that
 * matches a specified name.
 *
 * This function iterates through the child nodes of the specified parent
 * element \c (parentEl), looking for an element node whose tag name matches the
 * \c childName. If such a node is found, it is returned as an \c LDOM_Element.
 *
 * @param parentEl A constant reference to an \c LDOM_Element object
 * representing the parent element.
 * @param childName A \c std::string_view representing the name of the child
 * element to search for.
 * @return An \c LDOM_Element object representing the first matching child
 * element. If no matching child element is found, or if \c parentEl is null or
 * \c childName is empty, an empty \c LDOM_Element object is returned.
 */
LDOM_Element GetFirstChildByName(const LDOM_Element &parentEl,
                                 std::string_view childName);

/**
 * @brief Retrieves the value of a specified attribute from an element and
 * converts it to an integer.
 *
 * This function retrieves the value of an attribute (specified by \c attrName)
 * from the given \c elem as an LDOMString. It then attempts to convert this
 * string to an integer. If the string representation of the attribute is not
 * empty, the function uses \c std::stoi to convert it to an integer. If the
 * LDOMString type is LDOM_NULL or the string is empty, the function does not
 * modify the value.
 *
 * @param elem A constant reference to an \c LDOM_Element object.
 * @param attrName A constant reference to a \c std::string representing the
 * name of the attribute.
 * @param value A reference to an integer where the result will be stored.
 */
void GetAttributeAsInteger(const LDOM_Element &elem,
                           const std::string &attrName, int &value);

/**
 * @brief Retrieves the value of a specified attribute from an element and
 * converts it to a double.
 *
 * This function retrieves the value of an attribute (specified by `attrName`)
 * from the given `elem` as an LDOMString. It then attempts to convert this
 * string to a double. If the string representation of the attribute is not
 * empty, the function uses `strtod` to convert it to a double. If the
 * LDOMString type is LDOM_NULL or the string is empty, but represents an
 * integer, the function converts this integer to a double. If none of these
 * conditions are met, the function does not modify the value.
 *
 * @param elem A constant reference to an LDOM_Element object.
 * @param attrName A constant reference to a std::string representing the name
 * of the attribute.
 * @param value A reference to a double where the result will be stored.
 */
void GetAttributeAsDouble(const LDOM_Element &elem, const std::string &attrName,
                          double &value);

}  // namespace occutils::ldom
