#include "occutils/occutils-equality.h"

// OCC includes
#include <Precision.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Vec.hxx>
#include <gp_XY.hxx>
#include <gp_XYZ.hxx>

bool operator==(const gp_Pnt &a, const gp_Pnt &b) {
  return a.IsEqual(b, Precision::Confusion());
}

bool operator!=(const gp_Pnt &a, const gp_Pnt &b) { return !(a == b); }

bool operator==(const gp_XYZ &a, const gp_XYZ &b) {
  return a.IsEqual(b, Precision::Confusion());
}

bool operator!=(const gp_XYZ &a, const gp_XYZ &b) { return !(a == b); }

bool operator==(const gp_Pnt2d &a, const gp_Pnt2d &b) {
  return a.IsEqual(b, Precision::Confusion());
}

bool operator!=(const gp_Pnt2d &a, const gp_Pnt2d &b) { return !(a == b); }

bool operator==(const gp_XY &a, const gp_XY &b) {
  return a.IsEqual(b, Precision::Confusion());
}

bool operator!=(const gp_XY &a, const gp_XY &b) { return !(a == b); }

bool operator==(const gp_Vec &a, const gp_Vec &b) {
  return a.IsEqual(b, Precision::Confusion(), Precision::Angular());
}

bool operator!=(const gp_Vec &a, const gp_Vec &b) { return !(a == b); }
