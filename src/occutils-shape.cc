#include "occutils/occutils-shape.h"

// std includes
#include <vector>

// OCC includes
#include <BRepBndLib.hxx>
#include <BRepGProp.hxx>
#include <GProp_GProps.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

namespace occutils {

namespace shape {

bool IsCompound(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_COMPOUND;
}

bool IsCompoundSolid(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_COMPSOLID;
}

bool IsSolid(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_SOLID;
}

bool IsShell(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_SHELL;
}

bool IsFace(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_FACE;
}

bool IsWire(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_WIRE;
}

bool IsEdge(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_EDGE;
}

bool IsVertex(const TopoDS_Shape& shape) {
  return shape.ShapeType() == TopAbs_VERTEX;
}

//------------------------------------------------------------------------------

double Volume(const TopoDS_Shape& shape) {
  GProp_GProps gprops;
  BRepGProp::VolumeProperties(shape, gprops);
  return gprops.Mass();
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
