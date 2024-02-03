#include "occutils/occutils-ldom.h"

// std includes
#include <string>

// OCC includes
#include <LDOMString.hxx>
#include <LDOM_Element.hxx>

namespace occutils::ldom {

std::string GetLocalTagName(LDOM_Element const &el, bool keepPrefix) {
  std::string tagName = el.getTagName().GetString();
  if (std::size_t idx = tagName.find(':');
      idx != std::string::npos && !keepPrefix) {
    return tagName.substr(idx + 1);
  }
  return tagName;
}

//-----------------------------------------------------------------------------

std::string GetLocalAttrName(LDOM_Node const &el) {
  std::string tagName = el.getNodeName().GetString();
  if (std::size_t idx = tagName.find(':'); idx != std::string::npos) {
    return tagName.substr(idx + 1);
  }
  return tagName;
}

//-----------------------------------------------------------------------------

std::string GetAttrValue(LDOM_Element const &el, std::string_view name) {
  auto attributes = el.GetAttributesList();
  for (int i = 0; i < attributes.getLength(); i++) {
    LDOM_Node node = attributes.item(i);
    auto attrName = GetLocalAttrName(node);
    if (name == attrName) {
      return node.getNodeValue().GetString();
    }
  }
  return {};
}

//-----------------------------------------------------------------------------

std::string GetFirstChildTagName(LDOM_Element const &parentEl,
                                 bool keepPrefix) {
  auto aChildNode = parentEl.getFirstChild();
  if (const LDOM_Node::NodeType aChildNodeType = aChildNode.getNodeType();
      aChildNodeType == LDOM_Node::ELEMENT_NODE) {
    return GetLocalTagName((LDOM_Element &)aChildNode, keepPrefix);
  }
  return {};
}

//-----------------------------------------------------------------------------

LDOM_Element GetFirstChildByName(LDOM_Element const &parentEl,
                                 std::string_view childName) {
  if (parentEl == nullptr || childName.empty()) return {};

  for (LDOM_Node aChildNode = parentEl.getFirstChild(); !aChildNode.isNull();
       aChildNode = aChildNode.getNextSibling()) {
    if (aChildNode.getNodeType() == LDOM_Node::ELEMENT_NODE) {
      // Use C-style cast as a last resort to avoid compiler warnings.
      auto const aNextElement = (LDOM_Element &)(aChildNode);

      if (childName == GetLocalTagName(aNextElement)) return aNextElement;
    }
  }
  return {};
}

//-----------------------------------------------------------------------------

void GetAttributeAsInteger(LDOM_Element const &elem,
                           std::string const &attrName, int &value) {
  LDOMString stringValue = elem.getAttribute(attrName.c_str());
  if (stringValue.Type() == LDOMBasicString::StringType::LDOM_NULL) {
    return;
  }

  if (strlen(stringValue.GetString()) > 0) {
    value = std::stoi(stringValue.GetString());
    return;
  }

  stringValue.GetInteger(value);
}

//-----------------------------------------------------------------------------

void GetAttributeAsDouble(LDOM_Element const &elem, std::string const &attrName,
                          double &value) {
  LDOMString stringValue = elem.getAttribute(attrName.c_str());
  if (stringValue.Type() == LDOMBasicString::StringType::LDOM_NULL) {
    return;
  }

  if (strlen(stringValue.GetString()) > 0) {
    value = strtod(stringValue.GetString(), nullptr);
    return;
  }

  int i;
  stringValue.GetInteger(i);
  value = static_cast<double>(i);
}

}  // namespace occutils::ldom
