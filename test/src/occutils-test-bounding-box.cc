/***************************************************************************
 *   Created on: 14 Jun 202                                               *
 ***************************************************************************
 *   Copyright (c) 2024, Paul Buechner                                     *
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

// OCC includes
#include <BRepTools.hxx>
#include <Bnd_Box.hxx>
#include <Precision.hxx>
#include <TopoDS_Shape.hxx>

// occutils includes
#include "occutils/occutils-bounding-box.h"

using namespace occutils::bbox;

TEST(test_bounding_box, Is1DTest_FlatAlongAxis) {
  Bnd_Box bbox;
  const double tolerance = 1e-6;

  // Flat along X and Y axes
  bbox.Update(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  EXPECT_TRUE(Is1D(bbox, tolerance)) << "Flat along X and Y axes";

  // Flat along X and Z axes
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  EXPECT_TRUE(Is1D(bbox, tolerance)) << "Flat along X and Z axes";

  // Flat along Y and Z axes
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  EXPECT_TRUE(Is1D(bbox, tolerance)) << "Flat along Y and Z axes";

  // Not flat (3D bounding box)
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
  EXPECT_FALSE(Is1D(bbox, tolerance)) << "Not flat";

  // Flat along X-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
  EXPECT_FALSE(Is1D(bbox, tolerance)) << "Flat along X-axis only";

  // Flat along Y-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 1.0);
  EXPECT_FALSE(Is1D(bbox, tolerance)) << "Flat along Y-axis only";

  // Flat along Z-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
  EXPECT_FALSE(Is1D(bbox, tolerance)) << "Flat along Z-axis only";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, Is2DTest_FlatAlongAxis) {
  Bnd_Box bbox;
  const double tolerance = 1e-6;

  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
  EXPECT_TRUE(Is2D(bbox, tolerance)) << "Flat along X-axis";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 1.0);
  EXPECT_TRUE(Is2D(bbox, tolerance)) << "Flat along Y-axis";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
  EXPECT_TRUE(Is2D(bbox, tolerance)) << "Flat along Z-axis";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
  EXPECT_FALSE(Is2D(bbox, tolerance)) << "Not flat";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  EXPECT_FALSE(Is2D(bbox, tolerance)) << "Flat along X and Y axes";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  EXPECT_FALSE(Is2D(bbox, tolerance)) << "Flat along Y and Z axes";

  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  EXPECT_FALSE(Is2D(bbox, tolerance)) << "Flat along X and Z axes";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, Is3DTest_NotFlatAlongAnyAxis) {
  Bnd_Box bbox;
  const double tolerance = 1e-6;

  // 3D bounding box
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
  EXPECT_TRUE(Is3D(bbox, tolerance)) << "Not flat along any axis";

  // Flat along X-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 1.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along X-axis only";

  // Flat along Y-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 1.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along Y-axis only";

  // Flat along Z-axis only
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along Z-axis only";

  // Flat along X and Y axes
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along X and Y axes";

  // Flat along X and Z axes
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along X and Z axes";

  // Flat along Y and Z axes
  bbox.SetVoid();
  bbox.Update(0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
  EXPECT_FALSE(Is3D(bbox, tolerance)) << "Flat along Y and Z axes";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, ShapeTest_HandlesVoidBoundingBox) {
  // Create a void bounding box
  const Bnd_Box bbox;

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);

  // Check if the result is null
  ASSERT_TRUE(result.IsNull()) << "Result is null";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, ShapeTest_Handles1DBoundingBox) {
  // Create a 1D bounding box
  Bnd_Box bbox;
  bbox.Update(0.0, 0.0, 0.0, 0.0, 0.0, 1.0);  // Edge along the Z-axis

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_EDGE) << "Result is an edge";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, ShapeTest_Handles2DBoundingBox01) {
  // Create a 2D bounding box
  Bnd_Box bbox;
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0,
              0.0);  // Flat along the Z-axis (XY plane)

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);
  //
  BRepTools::Write(result, "generated/ShapeTest_Handles2DBoundingBox01.brep");

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_FACE) << "Result is a face";
}

TEST(test_bounding_box, ShapeTest_Handles2DBoundingBox02) {
  // Create a 2D bounding box
  Bnd_Box bbox;
  bbox.Update(56.32, -6, 1.2, 69.12, -4.4, 1.2);

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);
  //
  BRepTools::Write(result, "generated/ShapeTest_Handles2DBoundingBox02.brep");

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_FACE) << "Result is a face";
}

TEST(test_bounding_box, ShapeTest_Handles2DBoundingBox03) {
  // Create a 2D bounding box
  Bnd_Box bbox;
  bbox.Update(69, -10, 6.5, 69, 10, 7);

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);
  //
  BRepTools::Write(result, "generated/ShapeTest_Handles2DBoundingBox03.brep");

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_FACE) << "Result is a face";
}

TEST(test_bounding_box, ShapeTest_Handles2DBoundingBox04) {
  // Create a 2D bounding box
  Bnd_Box bbox;
  bbox.Update(56, 9.2, 1.1999999999999997, 69, 9.7, 1.2000000000000002);

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);
  //
  BRepTools::Write(result, "generated/ShapeTest_Handles2DBoundingBox04.brep");

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_FACE) << "Result is a face";
}

//------------------------------------------------------------------------------

TEST(test_bounding_box, ShapeTest_Handles3DBoundingBox) {
  // Create a 3D bounding box
  Bnd_Box bbox;
  bbox.Update(0.0, 0.0, 0.0, 1.0, 1.0, 1.0);

  const double tolerance = Precision::Confusion();

  // Call the function
  const TopoDS_Shape result = Shape(bbox, tolerance);

  // Check if the result is a face
  ASSERT_FALSE(result.IsNull()) << "Result is not null";
  ASSERT_TRUE(result.ShapeType() == TopAbs_SOLID) << "Result is a face";
}
