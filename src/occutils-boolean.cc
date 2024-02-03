#include "occutils/occutils-boolean.h"

// OCC includes
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>

// occutils includes
#include "occutils/occutils-list-utils.h"
#include "occutils/occutils-shape.h"

namespace occutils::boolean {

TopoDS_Shape Fuse(const TopTools_ListOfShape& arguments,
                  const TopTools_ListOfShape& tools) {
  if (arguments.Size() + tools.Size() == 1) {
    // Return that shape!
    if (arguments.Size() == 1) {
      return arguments.First();
    } else if (tools.Size() == 1) {
      return tools.First();
    } else {
      // Will never happen, just in case of hard issues to provide a hard return
      // path
      return {};
    }
  } else if (arguments.Size() + tools.Size() == 0) {
    // No shape => return no shape
    return {};
  } else if (arguments.Size() == 0) {
    throw std::invalid_argument("Fuse arguments must have at least one shape!");
  } else if (tools.Size() == 0) {
    throw std::invalid_argument("Fuse tools must have at least one shape!");
  }
  // Configure fuse
  BRepAlgoAPI_Fuse fuse;
  fuse.SetArguments(arguments);
  fuse.SetTools(tools);
  // Run fuse
  fuse.Build();
  return fuse.Shape();  // Raises NotDone if not done.
}

TopoDS_Shape Fuse(const TopTools_ListOfShape& shapes) {
  // We need "tools" and "arguments".
  // For fuse, the exact split does not matter,
  // but each must be size >= 1!
  auto [first, second] = list_utils::SplitIntoHeadAndTail(shapes, 1);
  return Fuse(second, first);
}

TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Shape>& shapes) {
  return Fuse(list_utils::ToOCCList(shapes));
}

TopoDS_Shape Cut(const TopTools_ListOfShape& positive,
                 const TopTools_ListOfShape& negative) {
  if (positive.Size() == 0) {
    throw std::invalid_argument("Cut positive must have at least one shape!");
  }
  if (negative.Size() == 0) {
    // Just fuse positive
    return Fuse(positive);
  }
  // Configure fuse
  BRepAlgoAPI_Cut cut;
  cut.SetArguments(positive);
  cut.SetTools(negative);
  // Run cut
  cut.Build();
  return cut.Shape();  // Raises NotDone if not done.
}

TopoDS_Shape Cut(const TopoDS_Shape& positive, const TopoDS_Shape& negative) {
  return Cut(list_utils::ToOCCList({positive}),
             list_utils::ToOCCList({negative}));
}

TopoDS_Shape Cut(const TopoDS_Shape& positive,
                 const TopTools_ListOfShape& negative) {
  return Cut(list_utils::ToOCCList({positive}), negative);
}

TopoDS_Shape Cut(const TopoDS_Shape& positive,
                 const std::initializer_list<TopoDS_Shape>& negative) {
  return Cut(list_utils::ToOCCList({positive}),
             list_utils::ToOCCList(negative));
}

TopoDS_Shape Cut(const std::vector<TopoDS_Solid>& positive,
                 const std::vector<TopoDS_Solid>& negative) {
  return Cut(shapes::FromSolids(positive), shapes::FromSolids(negative));
}

TopoDS_Shape Cut(const TopoDS_Solid& positive,
                 const std::vector<TopoDS_Solid>& negative) {
  return Cut({positive}, shapes::FromSolids(negative));
}

TopoDS_Shape Cut(const std::vector<TopoDS_Face>& positive,
                 const std::vector<TopoDS_Face>& negative) {
  return Cut(shapes::FromFaces(positive), shapes::FromFaces(negative));
}

TopoDS_Shape Cut(const TopoDS_Face& positive,
                 const std::vector<TopoDS_Face>& negative) {
  return Cut({positive}, shapes::FromFaces(negative));
}

TopoDS_Shape Fuse(const std::vector<TopoDS_Solid>& shapes) {
  return Fuse(shapes::FromSolids(shapes));
}

TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Solid>& shapes) {
  return Fuse(shapes::FromSolids(shapes));
}

TopoDS_Shape Fuse(const std::vector<TopoDS_Face>& shapes) {
  return Fuse(shapes::FromFaces(shapes));
}

TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Face>& shapes) {
  return Fuse(shapes::FromFaces(shapes));
}

}  // namespace occutils::boolean
