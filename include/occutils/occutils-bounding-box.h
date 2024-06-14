#pragma once

// std includes
#include <utility>
#include <vector>

// OCC includes
#include <Bnd_Box.hxx>
#include <Precision.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Vec.hxx>
#include <gp_XYZ.hxx>

namespace occutils::bbox {

/**
 * Compute the bounding box of the given shape,
 * represented by two corner coordinate vectors.
 *
 * The first returned vector contains xmin, ymin & zmin
 * The second returned vector contains xmax, ymax & zmax
 */
std::pair<gp_Vec, gp_Vec> BoundingBox(const TopoDS_Shape& shape);

/**
 * Compute the shape of the bounding box for the given shape
 */
gp_XYZ Size(const TopoDS_Shape& shape);

/**
 * Compute the volume of the bounding box of the given shape.
 */
double Volume(const TopoDS_Shape& shape);

/**
 * @brief Check if the given bounding box is edgey (1D).
 *
 * A bounding box is considered edgey if the difference between the min and max
 * coordinates in any two dimensions is less than the given tolerance.
 *
 * @param bbox The bounding box to check
 * @param tol The tolerance to determine if coordinates are equal
 * @return True if the bounding box is edgey, false otherwise
 */
bool Is1D(const Bnd_Box& bbox, double tol = Precision::Confusion());

/**
 * @brief Check if the given bounding box is flat (2D).
 *
 * A bounding box is considered flat if the difference between the min and max
 * coordinates in any (but no more than one) dimension is less than the given
 * tolerance.
 *
 * @param bbox The bounding box to check
 * @param tol The tolerance to determine if coordinates are equal
 * @return True if the bounding box is flat, false otherwise
 */
bool Is2D(const Bnd_Box& bbox, double tol = Precision::Confusion());

/**
 * @brief Check if the given bounding box is boxy (3D).
 *
 * A bounding box is considered boxy if the difference between the min and max
 * coordinates in all three dimensions is greater than the given tolerance.
 *
 * @param bbox The bounding box to check
 * @param tol The tolerance to determine if coordinates are equal
 * @return True if the bounding box is boxy, false otherwise
 */
bool Is3D(const Bnd_Box& bbox, double tol = Precision::Confusion());

/**
 * @brief Get the equivalent shape representation of the bounding box.
 *
 * The shape will dependent on the given bounding box dimensions:
 *   - 1D (two bbox corner coords match): a TopoDS_Edge
 *   - 2D (one bbox corner coords match): a TopoDS_Face
 *   - 3D: a TopoDS_Solid
 * If the bounding box is empty, the returned shape will be a null shape.
 *
 * @param bbox The bounding box to retrieve the shape from
 * @param tol The tolerance to determine if coordinates are equal
 * @return The equivalent shape representation of the bounding box
 */
TopoDS_Shape Shape(const Bnd_Box& bbox, double tol = Precision::Confusion());

}  // namespace occutils::bbox
