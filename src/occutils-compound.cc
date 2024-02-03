#include "occutils/occutils-compound.h"

// OCC includes
#include <BRep_Builder.hxx>
#include <TopoDS_Compound.hxx>

// occutils includes
#include "occutils/occutils-shape.h"

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

TopoDS_Compound From(const std::vector<TopoDS_Face>& faces) {
  return ToCompound(faces);
}

TopoDS_Compound From(const std::vector<TopoDS_Solid>& solids) {
  return ToCompound(solids);
}

}  // namespace occutils::compound
