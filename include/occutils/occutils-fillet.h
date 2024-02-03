#pragma once

// std includes
#include <numeric>

// OCC includes
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>

namespace occutils::fillet {

/**
 * Fillet all edges of the given shape using a single radius
 * and return the filleted shape.
 *
 * Usually the shape is a solid.
 */
TopoDS_Shape FilletAll(const TopoDS_Shape& shape, double radius = 1.0);

/**
 * Fillet all edges of the given shape using a
 * user-defined function to define the radius separately
 * for each edge.
 *
 * radiusByEdge takes the edge and should return either:
 *  - A radius
 *  - or NaN if this edge should NOT be filleted
 *
 * Usually the shape is a solid.
 */
template <typename RadiusFunc>
TopoDS_Shape FilletAdaptiveRadius(const TopoDS_Shape& shape,
                                  const RadiusFunc& radiusByEdge) {
  BRepFilletAPI_MakeFillet filletMaker(shape);
  // Iterate edges
  TopTools_IndexedMapOfShape edges;
  TopExp::MapShapes(shape, TopAbs_EDGE, edges);

  for (Standard_Integer i = 1; i <= edges.Extent(); i++) {
    const TopoDS_Edge& edge = TopoDS::Edge(edges(i));
    double radius = radiusByEdge(edge);
    if (!std::isnan(radius)) {  // NaN => dont fillet this edge!
      filletMaker.Add(radius, edge);
    }
  }
  filletMaker.Build();
  return filletMaker.Shape();
}

}  // namespace occutils::fillet
