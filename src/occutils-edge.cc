#include "occutils/occutils-edge.h"

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <gp_Circ.hxx>

#include "occutils/occutils-curve.h"
#include "occutils/occutils-equality.h"
#include "occutils/occutils-point.h"

namespace occutils::edge {

TopoDS_Edge FromPoints(const gp_Pnt& p1, const gp_Pnt& p2) {
  // Are the two points coincident?
  // If so, return an "empty" edge
  if (p1 == p2) {
    return {};
  }
  // Not the same => Create a linear edge
  return BRepBuilderAPI_MakeEdge(p1, p2).Edge();
}

TopoDS_Edge FullCircle(double radius) { return FullCircle(ax2::OZ(), radius); }

TopoDS_Edge FullCircle(const gp_Dir& direction, double radius) {
  return FullCircle(gp_Ax2(point::Origin(), direction), radius);
}

TopoDS_Edge FullCircle(const gp_Ax1& axis, double radius) {
  return FullCircle(ax2::FromAx1(axis), radius);
}

TopoDS_Edge FullCircle(const gp_Pnt& center, const gp_Dir& direction,
                       double radius) {
  return FullCircle(gp_Ax1(center, direction), radius);
}

TopoDS_Edge FullCircle(const gp_Ax2& axis, double radius) {
  return BRepBuilderAPI_MakeEdge(gp_Circ(axis, radius)).Edge();
}

double Length(const TopoDS_Edge& edge) {
  return curve::Length(curve::FromEdge(edge));
}

TopoDS_Edge CircleSegment(const gp_Ax2& axis, double radius, const gp_Pnt& p1,
                          const gp_Pnt& p2) {
  gp_Circ circ(axis, radius);
  return BRepBuilderAPI_MakeEdge(circ, p1, p2).Edge();
}

}  // namespace occutils::edge
