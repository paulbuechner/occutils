#include "occutils/occutils-bounding-box.h"

// std includes
#include <set>
#include <vector>

// OCC includes
#include <BRepBndLib.hxx>
#include <Bnd_Box.hxx>
#include <Bnd_OBB.hxx>
#include <TopoDS_Shape.hxx>
#include <gp_Vec.hxx>
#include <gp_XYZ.hxx>

// occutils includes
#include "occutils/occutils-edge.h"
#include "occutils/occutils-face.h"
#include "occutils/occutils-point.h"
#include "occutils/occutils-primitive.h"

namespace occutils::bbox {

std::pair<gp_Vec, gp_Vec> BoundingBox(const TopoDS_Shape& shape) {
  double xMin;
  double xMax;
  double yMin;
  double yMax;
  double zMin;
  double zMax;
  //
  Bnd_Box box;
  BRepBndLib::Add(shape, box);
  box.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  return std::make_pair(gp_Vec(xMin, yMin, zMin), gp_Vec(xMax, yMax, zMax));
}

//------------------------------------------------------------------------------

gp_XYZ BoundingBoxSize(const TopoDS_Shape& shape) {
  double xMin;
  double xMax;
  double yMin;
  double yMax;
  double zMin;
  double zMax;
  //
  Bnd_Box box;
  BRepBndLib::Add(shape, box);
  box.Get(xMin, yMin, zMin, xMax, yMax, zMax);

  return {abs(xMax - xMin), abs(yMax - yMin), abs(zMax - zMin)};
}

//------------------------------------------------------------------------------

double BoundingBoxVolume(const TopoDS_Shape& shape) {
  gp_XYZ bbox = BoundingBoxSize(shape);
  return bbox.X() * bbox.Y() * bbox.Z();
}

//------------------------------------------------------------------------------

bool Is1D(const Bnd_Box& bbox, double tolerance) {
  bool xFlat = bbox.IsXThin(tolerance);
  bool yFlat = bbox.IsYThin(tolerance);
  bool zFlat = bbox.IsZThin(tolerance);

  return (xFlat && yFlat && !zFlat) || (xFlat && !yFlat && zFlat) ||
         (!xFlat && yFlat && zFlat);
}

//------------------------------------------------------------------------------

bool Is2D(const Bnd_Box& bbox, double tolerance) {
  bool xFlat = bbox.IsXThin(tolerance);
  bool yFlat = bbox.IsYThin(tolerance);
  bool zFlat = bbox.IsZThin(tolerance);

  return (xFlat && !yFlat && !zFlat) || (!xFlat && !yFlat && zFlat) ||
         (!xFlat && yFlat && !zFlat);
}

//------------------------------------------------------------------------------

bool Is3D(const Bnd_Box& bbox, double tolerance) {
  bool xFlat = bbox.IsXThin(tolerance);
  bool yFlat = bbox.IsYThin(tolerance);
  bool zFlat = bbox.IsZThin(tolerance);

  return !xFlat && !yFlat && !zFlat;
}

//------------------------------------------------------------------------------

TopoDS_Shape Shape(const Bnd_Box& bbox, double tolerance) {
  if (bbox.IsVoid()) return {};  // empty bounding box

  // 1D bounding box
  if (Is1D(bbox, tolerance)) {
    return edge::FromPoints(bbox.CornerMin(), bbox.CornerMax());
  }

  // 2D bounding box
  if (Is2D(bbox, tolerance)) {
    // Use OBB to work out the face in 3D space
    Bnd_OBB obb(bbox);
    gp_Pnt corners[8];
    //
    obb.GetVertex(corners);

    // Remove "duplicate" points as the OBB may return the same point multiple
    // times due to the way the box is constructed
    std::set<gp_Pnt, point::Compare> unique(corners, corners + 8);
    std::vector<gp_Pnt> points(unique.begin(), unique.end());

    return face::FromPoints({points[0], points[1], points[3], points[2]});
  }

  // 3D bounding box
  return primitive::MakeBox(bbox.CornerMin(), bbox.CornerMax());
}

}  // namespace occutils::bbox
