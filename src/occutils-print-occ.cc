#include "occutils/occutils-print-occ.h"

// std includes
#include <ostream>

// OCC includes
#include <gp_Ax2.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>
#include <gp_XY.hxx>
#include <gp_XYZ.hxx>

std::ostream &operator<<(std::ostream &os, const gp_Pnt2d &xy) {
  return os << "gp_Pnt2d " << xy.XY();
}
std::ostream &operator<<(std::ostream &os, const gp_Pnt &xy) {
  return os << "gp_Pnt " << xy.XYZ();
}
std::ostream &operator<<(std::ostream &os, const gp_Dir2d &xy) {
  return os << "gp_Dir2d " << xy.XY();
}
std::ostream &operator<<(std::ostream &os, const gp_Dir &xy) {
  return os << "gp_Dir " << xy.XYZ();
}

std::ostream &operator<<(std::ostream &os, const gp_XY &xy) {
  return (os << '[' << xy.X() << ", " << xy.Y() << ']');
}

std::ostream &operator<<(std::ostream &os, const gp_XYZ &xyz) {
  return (os << '[' << xyz.X() << ", " << xyz.Y() << ", " << xyz.Z() << ']');
}

std::ostream &operator<<(std::ostream &os, const gp_Vec &vec) {
  return os << "gp_Vec " << vec.XYZ();
}
