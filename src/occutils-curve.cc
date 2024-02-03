#include "occutils/occutils-curve.h"

// std includes
#include <vector>

// OCC includes
#include <BRep_Tool.hxx>
#include <GCPnts_AbscissaPoint.hxx>
#include <GeomAbs_CurveType.hxx>

namespace occutils {

namespace curve {

GeomAdaptor_Curve FromEdge(const TopoDS_Edge& edge) {
  double umin;
  double umax;
  // Get unbounded curve plus separate bounding parameters
  auto rawCurve = BRep_Tool::Curve(edge, umin, umax);
  return {rawCurve, umin, umax};
}

GeomAdaptor_Curve FromTrimmedCurve(const Geom_TrimmedCurve& curve) {
  return {curve.BasisCurve(), curve.FirstParameter(), curve.LastParameter()};
}

double Length(const GeomAdaptor_Curve& curve) {
  return GCPnts_AbscissaPoint::Length(curve);
}

double Length(const Handle(Geom_Curve) & curve) {
  return Length(GeomAdaptor_Curve(curve));
}

double Length(const Geom_TrimmedCurve& curve) {
  return Length(FromTrimmedCurve(curve));
}

bool IsLine(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_Line;
}

bool IsCircle(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_Circle;
}

bool IsEllipse(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_Ellipse;
}

bool IsHyperbola(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_Hyperbola;
}

bool IsParabola(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_Parabola;
}

bool IsBezier(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_BezierCurve;
}

bool IsBSpline(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_BSplineCurve;
}

bool IsOffsetCurve(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_OffsetCurve;
}

bool IsOther(const GeomAdaptor_Curve& curve) {
  return curve.GetType() == GeomAbs_OtherCurve;
}

}  // namespace curve

namespace curves {

std::vector<GeomAdaptor_Curve> Only(
    const std::vector<GeomAdaptor_Curve>& curves, GeomAbs_CurveType type) {
  return Filter(curves, [type](const GeomAdaptor_Curve& curve) {
    return curve.GetType() == type;
  });
}

}  // namespace curves

}  // namespace occutils
