#include "occutils/occutils-fillet.h"

// OCC includes
#include <BRepFilletAPI_MakeFillet.hxx>
#include <NCollection_IndexedMap.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

namespace occutils::fillet
{

TopoDS_Shape FilletAll(const TopoDS_Shape& shape, const double radius)
{
  BRepFilletAPI_MakeFillet filletMaker(shape);
  // Iterate edges
  NCollection_IndexedMap<TopoDS_Shape, TopTools_ShapeMapHasher> edges;
  TopExp::MapShapes(shape, TopAbs_EDGE, edges);

  for (int i = 1; i <= edges.Extent(); i++)
  {
    const TopoDS_Edge& edge = TopoDS::Edge(edges(i));
    filletMaker.Add(radius, edge);
  }
  filletMaker.Build();
  return filletMaker.Shape();
}

} // namespace occutils::fillet
