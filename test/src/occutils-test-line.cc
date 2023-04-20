/***************************************************************************
 *   Created on: 19 Apr 2023                                               *
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

#include "occutils/occutils-direction.h"
#include "occutils/occutils-equality.h"
#include "occutils/occutils-line.h"
#include "occutils/occutils-print-occ.h"

TEST(LineTest, LineParallel2d) {
  // A line should be parallel to itself
  gp_Lin2d linX(gp_Pnt2d(0, 0), OCCUtils::Direction::X2d());
  EXPECT_TRUE(OCCUtils::Line::IsParallel(linX, linX));

  // A line should be parallel to its reverse line
  gp_Lin2d linMinusX = linX.Reversed();
  EXPECT_TRUE(OCCUtils::Line::IsParallel(linX, linMinusX));
  EXPECT_TRUE(OCCUtils::Line::IsParallel(linMinusX, linX));

  // Two perpendicular lines should not be parallel
  gp_Lin2d linY(gp_Pnt2d(0, 0), OCCUtils::Direction::Y2d());
  EXPECT_FALSE(OCCUtils::Line::IsParallel(linX, linY));
  EXPECT_FALSE(OCCUtils::Line::IsParallel(linY, linX));
  EXPECT_FALSE(OCCUtils::Line::IsParallel(linMinusX, linY));
  EXPECT_FALSE(OCCUtils::Line::IsParallel(linY, linMinusX));
}

TEST(LineTest, LineIntersection2D) {
  // Intersect between a line and itself has infinite points
  gp_Lin2d lin1(gp_Pnt2d(0, 0), OCCUtils::Direction::X2d());
  auto result = OCCUtils::Line::Intersection(lin1, lin1);
  EXPECT_FALSE(result.has_value());

  // Two intersecting lines should have an intersection point
  gp_Lin2d lin2(gp_Pnt2d(0, -1), OCCUtils::Direction::Y2d());
  result = OCCUtils::Line::Intersection(lin1, lin2);
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), gp_Pnt2d(0, 0));

  // Two more intersecting lines that do not intersect @ origin
  gp_Lin2d lin3(gp_Pnt2d(0, 1), OCCUtils::Direction::X2d());
  result = OCCUtils::Line::Intersection(lin2, lin3);
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), gp_Pnt2d(0, 1));

  // ... same as last test but argument order inverted (same result)
  result = OCCUtils::Line::Intersection(lin3, lin2);
  EXPECT_TRUE(result.has_value());
  EXPECT_EQ(result.value(), gp_Pnt2d(0, 1));
}