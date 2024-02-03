#include "occutils/occutils-fillet.h"

// OCC includes
#include <BRepFilletAPI_MakeFillet.hxx>
#include <TopExp.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>

namespace occutils::fillet {

TopoDS_Shape FilletAll(const TopoDS_Shape& shape, double radius) {
  BRepFilletAPI_MakeFillet filletMaker(shape);
  // Iterate edges
  TopTools_IndexedMapOfShape edges;
  TopExp::MapShapes(shape, TopAbs_EDGE, edges);

  for (Standard_Integer i = 1; i <= edges.Extent(); i++) {
    const TopoDS_Edge& edge = TopoDS::Edge(edges(i));
    filletMaker.Add(radius, edge);
  }
  filletMaker.Build();
  return filletMaker.Shape();
}

}  // namespace occutils::fillet
