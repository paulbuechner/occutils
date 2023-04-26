#include "occutils/occutils-face.h"

#include <BRepBuilderAPI_MakeFace.hxx>

#include "occutils/occutils-surface.h"
#include "occutils/occutils-wire.h"

namespace occutils::face {

TopoDS_Face FromWire(const TopoDS_Wire& wire) {
  if (wire.IsNull()) {
    return {};
  }
  BRepBuilderAPI_MakeFace faceMaker(wire);
  return faceMaker.IsDone() ? faceMaker.Face() : TopoDS_Face();
}

TopoDS_Face FromEdges(const std::initializer_list<TopoDS_Edge>& edges) {
  return FromWire(wire::FromEdges(edges));
}

TopoDS_Face FromEdges(const std::vector<TopoDS_Edge>& edges) {
  return FromWire(wire::FromEdges(edges));
}

TopoDS_Face FromEdge(const TopoDS_Edge& edge) {
  return face::FromWire(wire::FromEdges({edge}));
}

std::optional<gp_Ax1> Normal(const TopoDS_Face& face, double u, double v,
                             double precision) {
  auto surface = surface::FromFace(face);
  if (surface.Surface().IsNull()) {
    return std::nullopt;
  }
  return surface::Normal(surface, u, v, precision);
}

std::optional<gp_Dir> NormalDirection(const TopoDS_Face& face, double u,
                                      double v, double precision) {
  auto surface = surface::FromFace(face);
  if (surface.Surface().IsNull()) {
    return std::nullopt;
  }
  return surface::NormalDirection(surface, u, v, precision);
}

TopoDS_Face FromPoints(const std::vector<gp_Pnt>& points) {
  return FromWire(wire::FromPoints(points, true));
}

}  // namespace occutils::face
