#include "occutils/occutils-boolean.h"

// OCC includes
#include <BRepAlgoAPI_Common.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Section.hxx>

// occutils includes
#include "occutils/occutils-exceptions.h"
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
    throw OCCInvalidArgumentException(
        "Fuse arguments must have at least one shape!");
  } else if (tools.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Fuse tools must have at least one shape!");
  }
  // Configure fuse
  BRepAlgoAPI_Fuse fuse;
  fuse.SetArguments(arguments);
  fuse.SetTools(tools);
  // Run fuse
  fuse.Build();
  //
  if (bool hasErr = fuse.HasErrors(); hasErr) {
    return {};
  }
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

//------------------------------------------------------------------------------

TopoDS_Shape Cut(const TopTools_ListOfShape& positive,
                 const TopTools_ListOfShape& negative) {
  if (positive.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Cut positive must have at least one shape!");
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
  //
  if (bool hasErr = cut.HasErrors(); hasErr) {
    return {};
  }
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

//------------------------------------------------------------------------------

TopoDS_Shape Common(const TopTools_ListOfShape& arguments,
                    const TopTools_ListOfShape& tools) {
  if (arguments.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Common arguments must have at least one shape!");
  }
  if (tools.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Common tools must have at least one shape!");
  }
  // Configure common operation
  BRepAlgoAPI_Common common;
  common.SetArguments(arguments);
  common.SetTools(tools);
  // Run common operation
  common.Build();
  //
  if (bool hasErr = common.HasErrors(); hasErr) {
    return {};
  }
  return common.Shape();
}

TopoDS_Shape Common(const TopoDS_Shape& arguments, const TopoDS_Shape& tools) {
  return Cut(list_utils::ToOCCList({arguments}),
             list_utils::ToOCCList({tools}));
}

TopoDS_Shape Common(const TopoDS_Shape& arguments,
                    const TopTools_ListOfShape& tools) {
  return Common(list_utils::ToOCCList({arguments}), tools);
}

TopoDS_Shape Common(const TopoDS_Shape& arguments,
                    const std::initializer_list<TopoDS_Shape>& tools) {
  return Common(list_utils::ToOCCList({arguments}),
                list_utils::ToOCCList(tools));
}

TopoDS_Shape Common(const std::vector<TopoDS_Solid>& arguments,
                    const std::vector<TopoDS_Solid>& tools) {
  return Common(shapes::FromSolids(arguments), shapes::FromSolids(tools));
}

TopoDS_Shape Common(const TopoDS_Solid& arguments,
                    const std::vector<TopoDS_Solid>& tools) {
  return Common({arguments}, shapes::FromSolids(tools));
}

TopoDS_Shape Common(const std::vector<TopoDS_Face>& arguments,
                    const std::vector<TopoDS_Face>& tools) {
  return Common(shapes::FromFaces(arguments), shapes::FromFaces(tools));
}

TopoDS_Shape Common(const TopoDS_Face& arguments,
                    const std::vector<TopoDS_Face>& tools) {
  return Common({arguments}, shapes::FromFaces(tools));
}

//------------------------------------------------------------------------------

TopoDS_Shape Section(const TopTools_ListOfShape& arguments,
                     const TopTools_ListOfShape& tools) {
  if (arguments.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Section arguments must have at least one shape!");
  }
  if (tools.Size() == 0) {
    throw OCCInvalidArgumentException(
        "Section tools must have at least one shape!");
  }
  // Configure section operation
  BRepAlgoAPI_Section section;
  section.SetArguments(arguments);
  section.SetTools(tools);
  // Run section operation
  section.Build();
  //
  if (bool hasErr = section.HasErrors(); hasErr) {
    return {};
  }
  return section.Shape();
}

TopoDS_Shape Section(const TopoDS_Shape& arguments, const TopoDS_Shape& tools) {
  return Section(list_utils::ToOCCList({arguments}),
                 list_utils::ToOCCList({tools}));
}

TopoDS_Shape Section(const TopoDS_Shape& arguments,
                     const TopTools_ListOfShape& tools) {
  return Section(list_utils::ToOCCList({arguments}), tools);
}

TopoDS_Shape Section(const TopoDS_Shape& arguments,
                     const std::initializer_list<TopoDS_Shape>& tools) {
  return Section(list_utils::ToOCCList({arguments}),
                 list_utils::ToOCCList(tools));
}

TopoDS_Shape Section(const std::vector<TopoDS_Solid>& arguments,
                     const std::vector<TopoDS_Solid>& tools) {
  return Section(shapes::FromSolids(arguments), shapes::FromSolids(tools));
}

TopoDS_Shape Section(const TopoDS_Solid& arguments,
                     const std::vector<TopoDS_Solid>& tools) {
  return Section({arguments}, shapes::FromSolids(tools));
}

TopoDS_Shape Section(const std::vector<TopoDS_Face>& arguments,
                     const std::vector<TopoDS_Face>& tools) {
  return Section(shapes::FromFaces(arguments), shapes::FromFaces(tools));
}

TopoDS_Shape Section(const TopoDS_Face& arguments,
                     const std::vector<TopoDS_Face>& tools) {
  return Section({arguments}, shapes::FromFaces(tools));
}

}  // namespace occutils::boolean
