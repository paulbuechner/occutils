/***************************************************************************
 *   Created on: 29 Dec 2023                                               *
 ***************************************************************************
 *   Copyright (c) 2022, Paul Buechner                                     *
 *                                                                         *
 *   This file is part of the occutils library.                            *
 *                                                                         *
 *   This library is free software; you can redistribute it and/or         *
 *   modify it under the terms of the Apache License version 2.0 as        *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 ***************************************************************************/

// gtest includes
#include <gtest/gtest.h>

// std includes
#include <fstream>
#include <string>
#include <utility>
#include <vector>

// OCC includes
#include <LDOMParser.hxx>
#include <LDOM_Document.hxx>
#include <LDOM_Element.hxx>
#include <LDOM_Node.hxx>

// occutils includes
#include "occutils/occutils-ldom.h"

using namespace occutils::ldom;

class XMLParsingTest : public ::testing::Test {
 protected:
  LDOMParser myParser;
  LDOM_Document myDocument;
  std::string xmlFilePath;

  // Constructor to set the file path
  explicit XMLParsingTest(std::string filePath)
      : xmlFilePath(std::move(filePath)) {}

  // Set up the test environment
  void SetUp() override {
    std::ifstream fileStream(xmlFilePath);
    if (!fileStream.is_open()) {
      FAIL() << "Failed to open XML file: " << xmlFilePath;
    }

    // Parse the XML content
    myParser.parse(xmlFilePath.c_str());
    myDocument = myParser.getDocument();

    fileStream.close();
  }
};

// Example usage
class LibraryXMLTest : public XMLParsingTest {
 public:
  LibraryXMLTest() : XMLParsingTest("data/ldom/library.xml") {}
};

TEST_F(LibraryXMLTest, TestLibraryParsing) {
  // Get the root element
  LDOM_Element rootElement = myDocument.getDocumentElement();
  std::string rootTagName = GetLocalTagName(rootElement);
  std::string rootNodeName = GetLocalAttrName(rootElement);

  // Check if the root tag name is "library"
  EXPECT_EQ(rootTagName, "library");
  EXPECT_EQ(rootNodeName, "library");

  //
  // Get the first child element
  LDOM_Node firstChild = rootElement.getFirstChild();
  LDOM_Element firstChildEl = (LDOM_Element&)firstChild;

  // Get the tag name of the first child element
  std::string tagName = GetLocalTagName(firstChildEl);

  // Check if the tag name is "book"
  EXPECT_EQ(tagName, "book");

  // Test `GetAttrValue`
  // Get the value of the "id" attribute of the first child element
  std::string id = GetAttrValue(firstChildEl, "id");

  // Check if the id is "B001"
  EXPECT_EQ(id, "B001");

  // Test `GetFirstChildTagName`
  // Get the tag name of the first child element of the first child element
  std::string firstChildTagName = GetFirstChildTagName(rootElement, false);

  // Check if the tag name is "book"
  EXPECT_EQ(firstChildTagName, "book");

  // Test `GetFirstChildByName`
  // Get the first child element of the root element with the tag name "book"
  LDOM_Element firstChildByName = GetFirstChildByName(rootElement, "book");

  // Check if the element is valid (not null)
  EXPECT_FALSE(firstChildByName.isNull());
  // Check if the tag name is "book"
  EXPECT_EQ(GetLocalTagName(firstChildByName), "book");

  // Test `GetAttributeAsInteger`
  // Get the value of the "edition" attribute of the first child element
  int edition;
  GetAttributeAsInteger(firstChildEl, "edition", edition);

  // Check if the edition is 3
  EXPECT_EQ(edition, 3);

  // Test `GetAttributeAsDouble`
  // Get the value of the "id" attribute of the first child element
  double rating;
  GetAttributeAsDouble(firstChildEl, "rating", rating);

  // Check if the rating is 4.5
  EXPECT_EQ(rating, 4.5);
}

TEST_F(LibraryXMLTest, TestGetChildrenByName) {
  // Get the root element
  LDOM_Element rootElement = myDocument.getDocumentElement();

  // Test `GetChildrenByName`
  // Get the all child elements of the root element with the tag name "book"
  std::vector<LDOM_Element> children = GetChildrenByName(rootElement, "book");

  // Check if the number of child elements is 2
  EXPECT_EQ(children.size(), 2);
}
