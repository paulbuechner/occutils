#include "occutils/occutils-compound.h"

// OCC includes
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>

namespace occutils::compound {

template <typename T>
TopoDS_Compound ToCompound(const T& shapes) {
  BRep_Builder builder;
  TopoDS_Compound compound;
  builder.MakeCompound(compound);
  for (const auto& shape : shapes) {
    if (!shape.IsNull()) {
      builder.Add(compound, shape);
    }
  }
  return compound;
}

TopoDS_Compound From(const TopTools_ListOfShape& shapes) {
  return ToCompound(shapes);
}

TopoDS_Compound From(const std::vector<TopoDS_Shape>& shapes) {
  return ToCompound(shapes);
}

TopoDS_Compound From(const std::vector<TopoDS_Compound>& comps) {
  return ToCompound(comps);
}

TopoDS_Compound From(const std::vector<TopoDS_CompSolid>& compSolids) {
  return ToCompound(compSolids);
}

TopoDS_Compound From(const std::vector<TopoDS_Solid>& solids) {
  return ToCompound(solids);
}

TopoDS_Compound From(const std::vector<TopoDS_Shell>& shells) {
  return ToCompound(shells);
}

TopoDS_Compound From(const std::vector<TopoDS_Face>& faces) {
  return ToCompound(faces);
}

TopoDS_Compound From(const std::vector<TopoDS_Wire>& wires) {
  return ToCompound(wires);
}

TopoDS_Compound From(const std::vector<TopoDS_Edge>& edges) {
  return ToCompound(edges);
}

TopoDS_Compound From(const std::vector<TopoDS_Vertex>& vertices) {
  return ToCompound(vertices);
}

}  // namespace occutils::compound
