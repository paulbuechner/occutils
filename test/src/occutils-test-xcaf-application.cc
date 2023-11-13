/***************************************************************************
 *   Created on: 13 Nov 2023                                               *
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

#include <gtest/gtest.h>

#include <BRepPrimAPI_MakeBox.hxx>

#include "occutils/xcaf/occutils-xcaf-application.h"
#include "occutils/xcaf/occutils-xcaf-material.h"

using namespace occutils::xcaf;

class ExtendedXCAFApplicationTest : public ::testing::Test {
 protected:
  ExtendedXCAFApplication m_app;  // Instance of class to be tested

  TDF_Label m_defaultMaterialLabel;  // Label of default material

  void SetUp() override {
    // Add a default material to the application context
    XCAFMaterial defaultMaterial("Steel", "High-grade steel", 7.85, "kg/m^3",
                                 "Density");
    m_defaultMaterialLabel = m_app.FindOrCreateMaterial(defaultMaterial);
  }

  void TearDown() override {
    // Clean up after each test if necessary.
  }
};

TEST_F(ExtendedXCAFApplicationTest, AddShapeWithProps) {
  // Create a simple box shape
  TopoDS_Shape boxShape = BRepPrimAPI_MakeBox(10.0, 10.0, 10.0).Shape();

  // Define shape properties
  XCAFShapeProperties props;
  props.SetColor(Quantity_ColorRGBA(Quantity_NOC_RED, 0.1f));  // Example color
  props.SetName("TestBox");

  // Call the method under test
  TDF_Label shapeLabel = m_app.AddShapeWithProps(boxShape, props);

  // Assert - Verify that the label is valid
  ASSERT_FALSE(shapeLabel.IsNull());

  // Assert - Check if the shape can be written to STEP
  ASSERT_TRUE(m_app.WriteSTEP("generated/STEP/box_with_props.stp"));
}

TEST_F(ExtendedXCAFApplicationTest, FindExistingMaterial) {
  // Setup - Create a material that exists in the application context
  XCAFMaterial existingMaterial("Steel", "High-grade steel", 7.85, "kg/m^3",
                                "Density");

  // Act - Try to find or create this material
  TDF_Label label = m_app.FindOrCreateMaterial(existingMaterial);

  // Assert - Check if the returned label is valid and points to the existing
  // material
  ASSERT_FALSE(label.IsNull());

  // Assert - Check if the returned label is valid and points to the existing
  // material
  ASSERT_EQ(label, m_defaultMaterialLabel);

  // Assert - Check if the created label size is 1
  ASSERT_EQ(m_app.GetMaterials().size(), 1);
}

TEST_F(ExtendedXCAFApplicationTest, CreateNewMaterial) {
  // Setup - Define a new material that does not exist
  XCAFMaterial newMaterial("NewMaterial", "Description", 1.23, "unit",
                           "valueType");

  // Act - Try to find or create this material
  TDF_Label label = m_app.FindOrCreateMaterial(newMaterial);

  // Assert - Check if the returned label is valid and points to the new
  // material
  ASSERT_FALSE(label.IsNull());

  // Assert - Check if the created label size is 2
  ASSERT_EQ(m_app.GetMaterials().size(), 2);
}

TEST_F(ExtendedXCAFApplicationTest, ReadWriteSTEP) {
  // Read the STEP file
  ASSERT_TRUE(m_app.ReadSTEP("data/STEP/as1-oc-214.stp"));

  // Write the STEP file
  ASSERT_TRUE(m_app.WriteSTEP("generated/STEP/as1-oc-214.stp"));
}
