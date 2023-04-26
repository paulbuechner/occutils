#include "occutils/occutils-axis.h"

#include <gp_Lin.hxx>

#include "occutils/occutils-direction.h"
#include "occutils/occutils-point.h"

namespace occutils {

namespace axis {

bool Contains(const gp_Ax1& axis, const gp_Pnt& pnt, double tolerance) {
  return gp_Lin(axis).Contains(pnt, tolerance);
}

double Distance(const gp_Ax1& axis, const gp_Pnt& pnt) {
  return gp_Lin(axis).Distance(pnt);
}

gp_Ax1 operator+(const gp_Ax1& axis, const gp_Vec& vec) {
  return {axis.Location() + vec, axis.Direction()};
}

gp_Ax1 operator+(const gp_Ax1& axis, const gp_XYZ& vec) {
  return {axis.Location() + vec, axis.Direction()};
}

}  // namespace axis

namespace ax1 {

gp_Ax1 OX() { return {occutils::point::Origin(), occutils::direction::X()}; }

gp_Ax1 OY() { return {occutils::point::Origin(), occutils::direction::Y()}; }

gp_Ax1 OZ() { return {occutils::point::Origin(), occutils::direction::Z()}; }

gp_Ax1 OMinusX() {
  return {occutils::point::Origin(), occutils::direction::MinusX()};
}

gp_Ax1 OMinusY() {
  return {occutils::point::Origin(), occutils::direction::MinusY()};
}

gp_Ax1 OMinusZ() {
  return {occutils::point::Origin(), occutils::direction::MinusZ()};
}

}  // namespace ax1

namespace ax2 {

gp_Ax2 FromAx1(const gp_Ax1& axis) {
  return {axis.Location(), axis.Direction()};
}

gp_Ax2 OX() { return {occutils::point::Origin(), occutils::direction::X()}; }

gp_Ax2 OY() { return {occutils::point::Origin(), occutils::direction::Y()}; }

gp_Ax2 OZ() { return {occutils::point::Origin(), occutils::direction::Z()}; }

gp_Ax2 OMinusX() {
  return {occutils::point::Origin(), occutils::direction::MinusX()};
}

gp_Ax2 OMinusY() {
  return {occutils::point::Origin(), occutils::direction::MinusY()};
}

gp_Ax2 OMinusZ() {
  return {occutils::point::Origin(), occutils::direction::MinusZ()};
}

}  // namespace ax2

}  // namespace occutils
