#include "occutils/occutils-shape.h"

// OCC includes
#include <utility>
#include <vector>

// OCC includes
#include <BRepBndLib.hxx>
#include <BRepGProp.hxx>
#include <Bnd_Box.hxx>
#include <GProp_GProps.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>
#include <gp_Vec.hxx>
#include <gp_XYZ.hxx>

namespace occutils {

namespace shape {

bool IsSolid(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_SOLID;
}

bool IsFace(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_FACE;
}

bool IsShell(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_SHELL;
}

bool IsEdge(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_EDGE;
}

bool IsWire(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_WIRE;
}

bool IsVertex(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_VERTEX;
}

double Volume(const TopoDS_Shape& shape) {
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(shape, gprops);
  return gprops.Mass();
}

std::pair<gp_Vec, gp_Vec> BoundingBox(const TopoDS_Shape& shape) {
  double xmin;
  double xmax;
  double ymin;
  double ymax;
  double zmin;
  double zmax;
  //
  Bnd_Box box;
  BRepBndLib::Add(shape, box);
  box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  return std::make_pair(gp_Vec(xmin, ymin, zmin), gp_Vec(xmax, ymax, zmax));
}

gp_XYZ BoundingBoxSize(const TopoDS_Shape& shape) {
  double xmin;
  double xmax;
  double ymin;
  double ymax;
  double zmin;
  double zmax;
  //
  Bnd_Box box;
  BRepBndLib::Add(shape, box);
  box.Get(xmin, ymin, zmin, xmax, ymax, zmax);

  return {abs(xmax - xmin), abs(ymax - ymin), abs(zmax - zmin)};
}

double BoundingBoxVolume(const TopoDS_Shape& shape) {
  gp_XYZ bbox = BoundingBoxSize(shape);
  return bbox.X() * bbox.Y() * bbox.Z();
}

}  // namespace shape

namespace shapes {

/**
 * Internal converter function
 */
template <typename T>
std::vector<TopoDS_Shape> ToShapes(const std::vector<T>& elems) {
  // Create return vector
  std::vector<TopoDS_Shape> ret;
  ret.reserve(elems.size());
  // Do the copying
  std::copy(elems.begin(), elems.end(), std::back_inserter(ret));
  return ret;
}

std::vector<TopoDS_Shape> FromSolids(const std::vector<TopoDS_Solid>& solids) {
  return ToShapes(solids);
}

std::vector<TopoDS_Shape> FromFaces(const std::vector<TopoDS_Face>& faces) {
  return ToShapes(faces);
}

}  // namespace shapes

}  // namespace occutils
