#include "occutils/occutils-direction.h"

// OCC includes
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Vec.hxx>
#include <gp_XY.hxx>
#include <gp_XYZ.hxx>

gp_Vec operator*(const gp_Dir& a, double factor) {
  return {a.X() * factor, a.Y() * factor, a.Z() * factor};
}

namespace occutils::direction {

gp_Dir X() { return {gp_XYZ(1.0, 0.0, 0.0)}; }

gp_Dir Y() { return {gp_XYZ(0.0, 1.0, 0.0)}; }

gp_Dir Z() { return {gp_XYZ(0.0, 0.0, 1.0)}; }

gp_Dir MinusX() { return {gp_XYZ(-1.0, 0.0, 0.0)}; }

gp_Dir MinusY() { return {gp_XYZ(0.0, -1.0, 0.0)}; }

gp_Dir MinusZ() { return {gp_XYZ(0.0, 0.0, -1.0)}; }

/**
 * Get the global 2D X axis direction
 */
gp_Dir2d X2d() { return {gp_XY(1.0, 0.0)}; }
/**
 * Get the global 2D -X axis direction
 */
gp_Dir2d MinusX2d() { return {gp_XY(-1.0, 0.0)}; }
/**
 * Get the global 2D Y axis direction
 */
gp_Dir2d Y2d() { return {gp_XY(0.0, 1.0)}; }
/**
 * Get the global 2D -Y axis direction
 */
gp_Dir2d MinusY2d() { return {gp_XY(0.0, -1.0)}; }

gp_Dir Orthogonal(const gp_Dir& a, const gp_Dir& b) { return a.Crossed(b); }

}  // namespace occutils::direction
