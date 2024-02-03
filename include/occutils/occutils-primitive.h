#pragma once

/**
 * Create boundary representation (BRep) primitives
 */

// OCC includes
#include <TopoDS_Solid.hxx>
#include <gp_Pnt.hxx>

namespace occutils::primitive {

/**
 * Configure how a primitive is centered in the coordinate system
 */
enum class PositionCentering {
  DoNotCenter,
  CenterX,
  CenterY,
  CenterZ,
  CenterL,
  CenterD
};

/**
 * Configure in which axis a primitive is oriented.
 * Defines the direction of the primitive's main axis.
 */
enum class Orientation { X, Y, Z };

/**
 * Make a box that can be centered on all axes individually.
 * @param xSize The dimension in the X dimension
 * @param ySize The dimension in the Y dimension
 * @param zSize The dimension in the Z dimension
 * @param center A set flag causes the box to be center in that dimension on the
 * origin (default is 0 - DoNotCenter). Allowed values are: 1 - CenterX, 2 -
 * CenterY, 3 - CenterZ.
 * @param origin The point where to create the box. Default is (0,0,0)
 */
TopoDS_Solid MakeBox(double xSize, double ySize, double zSize,
                     PositionCentering center = PositionCentering::DoNotCenter,
                     gp_Pnt origin = gp_Pnt());

/**
 * Make a box that stretches from point a to point b.
 */
TopoDS_Solid MakeBox(const gp_Pnt& a, const gp_Pnt& b);
TopoDS_Solid MakeBox(const gp_Vec& a, const gp_Vec& b);
TopoDS_Solid MakeBox(const std::pair<gp_Pnt, gp_Pnt>& ab);
TopoDS_Solid MakeBox(const std::pair<gp_Vec, gp_Vec>& ab);

/**
 * Make a cube that can be centered on all axes individually.
 *
 * @param size The dimension in the X/Y/Z dimensions
 * @param center A set flag causes the box to be center in that dimension on the
 * origin (default is 0 - DoNotCenter). Allowed values are: 1 - CenterX, 2 -
 * CenterY, 3 - CenterZ.
 * @param origin The point where to create the box. Default is (0,0,0)
 */
TopoDS_Solid MakeCube(double size,
                      PositionCentering center = PositionCentering::DoNotCenter,
                      gp_Pnt origin = gp_Pnt());

/**
 * Make a cone.
 *
 * @param diameter1 The diameter of the cone at the origin point
 * @param diameter2 The diameter of the cone opposite to the origin point
 * @param length The total length of the cone
 * @param axis Defines both the origin point and the axis of the cone
 * @param centerLength Whether to center the cone on its length
 */
TopoDS_Solid MakeCone(const gp_Ax1& axis, double diameter1, double diameter2,
                      double length, bool centerLength = false);

/**
 * Make a cylinder that can be centered.
 *
 * @param diameter The diameter of the cylinder
 * @param length The length of the cylinder
 * @param orientation How the cylinder's main axis is oriented
 * @param center A set flag causes the box to be center in that dimension on the
 * origin (default is 0 - DoNotCenter). Allowed values are: 4 - CenterL, 5 -
 * CenterD. If CenterL is chosen, the origin point is adjusted based on the
 * cylinder's diameter, centering the cylinder along the local axis specified by
 * the orientation. If CenterD is chosen, the origin point is adjusted based on
 * the cylinder's diameter and length, centering the cylinder along the diagonal
 * of its bounding box.
 * @param origin The point where to create the cylinder. Default is (0,0,0)
 */
TopoDS_Solid MakeCylinder(
    double diameter, double length, Orientation orientation = Orientation::Z,
    PositionCentering center = PositionCentering::DoNotCenter,
    gp_Pnt origin = gp_Pnt());

}  // namespace occutils::primitive
