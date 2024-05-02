#pragma once

// std includes
#include <initializer_list>
#include <vector>

// OCC includes
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

// occutils includes
#include "occutils/occutils-list-utils.h"

/**
 * Boolean operation utilities
 */
namespace occutils::boolean {

/**
 * Fuse two or more shapes in an OCC-style container.
 * Combines the shapes in the container into a single shape.
 *
 * @param shapes A list of shapes to be fused together.
 * @return A fused shape. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the list is empty.
 */
TopoDS_Shape Fuse(const TopTools_ListOfShape& shapes);

/**
 * Fuse with two lists of arguments.
 * Combines the shapes from both lists into a single shape.
 *
 * @param arguments A list of shapes to act as the main arguments for the
 * fusion.
 * @param tools A list of shapes to act as additional tools for the fusion.
 * @return A fused shape representing the union of both lists. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either list is empty.
 */
TopoDS_Shape Fuse(const TopTools_ListOfShape& arguments,
                  const TopTools_ListOfShape& tools);

/**
 * Fuse two or more shapes in an STL-like container.
 * Combines the shapes from the container into a single shape.
 *
 * @param shapes A container of shapes to be fused together.
 * @return A fused shape. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if there is only one shape in the
 * container.
 */
template <template <typename, typename> typename Container, typename Allocator>
TopoDS_Shape Fuse(const Container<TopoDS_Shape, Allocator>& shapes) {
  return Fuse(occutils::list_utils::ToOCCList(shapes));
}

/**
 * Fuse two or more shapes in an OCC-style container.
 * Combines the shapes from the container into a single shape.
 *
 * @param shapes An OCC-style container of shapes to be fused together.
 * @return A fused shape. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if there is only one shape in the
 * container.
 */
template <template <typename> typename Container>
TopoDS_Shape Fuse(const Container<TopoDS_Shape>& shapes) {
  return Fuse(occutils::list_utils::ToOCCList(shapes));
}

/**
 * Fuse two or more shapes in an OCC-style container.
 * Combines the shapes from the initializer list into a single shape.
 *
 * Convenience call function:
 *      Fuse({shape1, shape2})
 *      Fuse({shape1, shape2, shape3})
 *
 * @param shapes An initializer list of shapes to be fused together.
 * @return A fused shape. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if there is only one shape in the list.
 */
TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Shape>& shapes);

/**
 * Fuse two or more shapes in a vector of TopoDS_Solids.
 * Combines the shapes from the vector into a single shape.
 *
 * @param shapes A vector of TopoDS_Solids to be fused together.
 * @return A fused shape representing the combination of all solids. Check if
 * the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the vector is empty or contains only
 * one solid.
 */
TopoDS_Shape Fuse(const std::vector<TopoDS_Solid>& shapes);

/**
 * Fuse two or more shapes in an initializer list of TopoDS_Solids.
 * Combines the shapes from the initializer list into a single shape.
 *
 * @param shapes An initializer list of TopoDS_Solids to be fused together.
 * @return A fused shape representing the combination of all solids. Check if
 * the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the list is empty or contains only one
 * solid.
 */
TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Solid>& shapes);

/**
 * Fuse two or more shapes in a vector of TopoDS_Faces.
 * Combines the faces from the vector into a single shape.
 *
 * @param shapes A vector of TopoDS_Faces to be fused together.
 * @return A fused shape representing the combination of all faces. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the vector is empty or contains only
 * one face.
 */
TopoDS_Shape Fuse(const std::vector<TopoDS_Face>& shapes);

/**
 * Fuse two or more shapes in an initializer list of TopoDS_Faces.
 * Combines the faces from the initializer list into a single shape.
 *
 * @param shapes An initializer list of TopoDS_Faces to be fused together.
 * @return A fused shape representing the combination of all faces. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the list is empty or contains only one
 * face.
 */
TopoDS_Shape Fuse(const std::initializer_list<TopoDS_Face>& shapes);

//------------------------------------------------------------------------------

/**
 * Boolean subtraction between two lists of shapes.
 * Subtracts the negative shapes from the positive shapes.
 * positive represents the arguments, and negative represents the tools.
 *
 * @param positive A list of shapes to act as the main arguments for the
 * operation.
 * @param negative A list of shapes to act as the tools for the operation.
 * @return A shape representing the difference between the positive and negative
 * lists. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either list is empty.
 */
TopoDS_Shape Cut(const TopTools_ListOfShape& positive,
                 const TopTools_ListOfShape& negative);

/**
 * Boolean subtraction between two shapes.
 * Subtracts the negative shape from the positive shape.
 * positive represents the main argument, and negative represents the tool.
 *
 * @param positive The primary shape to act as the main argument for the
 * operation.
 * @param negative The secondary shape to act as the tool for the operation.
 * @return A shape representing the difference between the two shapes. Check if
 * the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null.
 */
TopoDS_Shape Cut(const TopoDS_Shape& positive, const TopoDS_Shape& negative);

/**
 * Boolean subtraction between a shape and a list of arguments.
 * Subtracts the negative shapes from the positive shape.
 * positive represents the main argument, and negative represents the tools.
 *
 * @param positive The primary shape to act as the main argument for the
 * operation.
 * @param negative A list of shapes to act as the tools for the operation.
 * @return A shape representing the difference between the positive shape and
 * the list of negative shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the positive shape is null or the list
 * of negative shapes is empty.
 */
TopoDS_Shape Cut(const TopoDS_Shape& positive,
                 const TopTools_ListOfShape& negative);

/**
 * Boolean subtraction between a shape and an initializer list of arguments.
 * Subtracts the negative shapes from the positive shape.
 * positive represents the main argument, and negative represents the tools.
 *
 * @param positive The primary shape to act as the main argument for the
 * operation.
 * @param negative An initializer list of shapes to act as the tools for the
 * operation.
 * @return A shape representing the difference between the positive shape and
 * the initializer list of negative shapes. Check if the shape is null before
 * using it.
 *
 * @throws OCCInvalidArgumentException if the positive shape is null or the list
 * of negative shapes is empty.
 */
TopoDS_Shape Cut(const TopoDS_Shape& positive,
                 const std::initializer_list<TopoDS_Shape>& negative);

/**
 * Boolean subtraction between a shape and a variable shape container of
 * arguments. Subtracts the negative shapes from the positive shape. positive
 * represents the main argument, and negative represents the tools.
 *
 * @param positive The primary shape to act as the main argument for the
 * operation.
 * @param negative A container of shapes to act as the tools for the operation.
 * The container must be templated with a type and an allocator.
 * @return A shape representing the difference between the positive shape and
 * the container of negative shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the positive shape is null or the
 * container of negative shapes is empty.
 */
template <template <typename, typename> typename Container, typename Allocator>
TopoDS_Shape Cut(const TopoDS_Shape& positive,
                 const Container<TopoDS_Shape, Allocator>& negative) {
  return Cut(occutils::list_utils::ToOCCList({positive}),
             occutils::list_utils::ToOCCList(negative));
}

/**
 * Boolean subtraction between two variable shape containers of arguments.
 * Subtracts the negative container from the positive container.
 * positive represents the main arguments, and negative represents the tools.
 *
 * @param positive A container of shapes to act as the main arguments for the
 * operation. The container must be templated with a type and an allocator.
 * @param negative A container of shapes to act as the tools for the operation.
 * The container must be templated with a type and an allocator.
 * @return A shape representing the difference between the two containers of
 * shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either container is empty.
 */
template <template <typename, typename> typename Container, typename Allocator1,
          typename Allocator2>
TopoDS_Shape Cut(const Container<TopoDS_Shape, Allocator1>& positive,
                 const Container<TopoDS_Shape, Allocator2>& negative) {
  return Cut(occutils::list_utils::ToOCCList(positive),
             occutils::list_utils::ToOCCList(negative));
}

/**
 * Boolean subtraction between two vectors of TopoDS_Solids.
 * This is a common use-case, hence we provide a utility function.
 * Subtracts the negative solids from the positive solids.
 * positive represents the arguments, and negative represents the tools.
 *
 * @param positive A vector of TopoDS_Solids to act as the main arguments for
 * the operation.
 * @param negative A vector of TopoDS_Solids to act as the tools for the
 * operation.
 * @return A shape representing the difference between the two vectors of
 * solids. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Cut(const std::vector<TopoDS_Solid>& positive,
                 const std::vector<TopoDS_Solid>& negative);

/**
 * Boolean subtraction between a TopoDS_Solid and a vector of TopoDS_Solids.
 * This is a common use-case, hence we provide a utility function.
 * Subtracts the negative solids from the positive solid.
 * positive represents the main argument, and negative represents the tools.
 *
 * @param positive A TopoDS_Solid to act as the main argument for the operation.
 * @param negative A vector of TopoDS_Solids to act as the tools for the
 * operation.
 * @return A shape representing the difference between the input solid and the
 * vector of solids. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the positive solid is null or the
 * vector of negative solids is empty.
 */
TopoDS_Shape Cut(const TopoDS_Solid& positive,
                 const std::vector<TopoDS_Solid>& negative);

/**
 * Boolean subtraction between two vectors of TopoDS_Faces.
 * This is a common use-case, hence we provide a utility function.
 * Subtracts the negative faces from the positive faces.
 * positive represents the arguments, and negative represents the tools.
 *
 * @param positive A vector of TopoDS_Faces to act as the main arguments for the
 * operation.
 * @param negative A vector of TopoDS_Faces to act as the tools for the
 * operation.
 * @return A shape representing the difference between the two vectors of faces.
 * Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Cut(const std::vector<TopoDS_Face>& positive,
                 const std::vector<TopoDS_Face>& negative);

/**
 * Boolean subtraction between a TopoDS_Face and a vector of TopoDS_Faces.
 * This is a common use-case, hence we provide a utility function.
 * Subtracts the negative faces from the positive face.
 * positive represents the main argument, and negative represents the tools.
 *
 * @param positive A TopoDS_Face to act as the main argument for the operation.
 * @param negative A vector of TopoDS_Faces to act as the tools for the
 * operation.
 * @return A shape representing the difference between the input face and the
 * vector of faces. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the positive face is null or the
 * vector of negative faces is empty.
 */
TopoDS_Shape Cut(const TopoDS_Face& positive,
                 const std::vector<TopoDS_Face>& negative);

//------------------------------------------------------------------------------

/**
 * Boolean intersection with two lists of arguments.
 * Finds the intersection between arguments and tools shapes.
 *
 * @param arguments A list of shapes to act as the main arguments for the
 * operation.
 * @param tools A list of shapes to act as the tools for the operation.
 * @return A shape representing the intersection of the two lists. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either list is empty.
 */
TopoDS_Shape Common(const TopTools_ListOfShape& arguments,
                    const TopTools_ListOfShape& tools);

/**
 * Boolean intersection between two shapes.
 * Finds the intersection between arguments and tools shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools The secondary shape to act as the tool for the operation.
 * @return A shape representing the intersection of the two input shapes. Check
 * if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null.
 */
TopoDS_Shape Common(const TopoDS_Shape& arguments, const TopoDS_Shape& tools);

/**
 * Boolean intersection between a shape and a list of shapes.
 * Finds the intersection between a arguments shape and tools shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools A list of shapes to act as the tools for the operation.
 * @return A shape representing the intersection of the input shape and list of
 * shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null or list is empty.
 */
TopoDS_Shape Common(const TopoDS_Shape& arguments,
                    const TopTools_ListOfShape& tools);

/**
 * Boolean intersection between a shape and an initializer list of shapes.
 * Finds the intersection between a arguments shape and tools shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools An initializer list of shapes to act as the tools for the
 * operation.
 * @return A shape representing the intersection of the input shape and
 * initializer list of shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null or list is empty.
 */
TopoDS_Shape Common(const TopoDS_Shape& arguments,
                    const std::initializer_list<TopoDS_Shape>& tools);

/**
 * Boolean intersection between a shape and a variable shape container of
 * arguments. Finds the intersection between a arguments shape and tools
 * shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools A container of shapes to act as the tools for the operation.
 * The container must be templated with a type and an allocator.
 * @return A shape representing the intersection of the input shape and
 * container of shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null or container is
 * empty.
 */
template <template <typename, typename> typename Container, typename Allocator>
TopoDS_Shape Common(const TopoDS_Shape& arguments,
                    const Container<TopoDS_Shape, Allocator>& tools) {
  return Common(occutils::list_utils::ToOCCList({arguments}),
                occutils::list_utils::ToOCCList(tools));
}

/**
 * Boolean intersection between two variable shape containers of arguments.
 * Finds the intersection between arguments and tools shapes.
 *
 * @param arguments A container of shapes to act as the main arguments for the
 * operation. The container must be templated with a type and an allocator.
 * @param tools A container of shapes to act as the tools for the operation.
 * The container must be templated with a type and an allocator.
 * @return A shape representing the intersection of the two containers of
 * shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either container is empty.
 */
template <template <typename, typename> typename Container, typename Allocator1,
          typename Allocator2>
TopoDS_Shape Common(const Container<TopoDS_Shape, Allocator1>& arguments,
                    const Container<TopoDS_Shape, Allocator2>& tools) {
  return Common(occutils::list_utils::ToOCCList(arguments),
                occutils::list_utils::ToOCCList(tools));
}

/**
 * Boolean intersection between two vectors of TopoDS_Solids.
 * Finds the intersection between arguments and tools solids.
 *
 * @param arguments A vector of TopoDS_Solids to act as the main arguments for
 * the operation.
 * @param tools A vector of TopoDS_Solids to act as the tools for the
 * operation.
 * @return A shape representing the intersection of the two vectors of solids.
 * Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Common(const std::vector<TopoDS_Solid>& arguments,
                    const std::vector<TopoDS_Solid>& tools);

/**
 * Boolean intersection between a TopoDS_Solid and a vector of TopoDS_Solids.
 * Finds the intersection between arguments and tools solids.
 *
 * @param arguments A TopoDS_Solid to act as the main argument for the
 * operation.
 * @param tools A vector of TopoDS_Solids to act as the tools for the
 * operation.
 * @return A shape representing the intersection of the input solid and vector
 * of solids. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either TopoDS_Solid is null or vector
 * is empty.
 */
TopoDS_Shape Common(const TopoDS_Solid& arguments,
                    const std::vector<TopoDS_Solid>& tools);

/**
 * Boolean intersection between two vectors of TopoDS_Faces.
 * Finds the intersection between arguments and tools faces.
 *
 * @param arguments A vector of TopoDS_Faces to act as the main arguments for
 * the operation.
 * @param tools A vector of TopoDS_Faces to act as the tools for the
 * operation.
 * @return A shape representing the intersection of the two vectors of faces.
 * Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Common(const std::vector<TopoDS_Face>& arguments,
                    const std::vector<TopoDS_Face>& tools);

/**
 * Boolean intersection between a TopoDS_Face and a vector of TopoDS_Faces.
 * Finds the intersection between arguments and tools faces.
 *
 * @param arguments A TopoDS_Face to act as the main argument for the operation.
 * @param tools A vector of TopoDS_Faces to act as the tools for the
 * operation.
 * @return A shape representing the intersection of the input face and vector of
 * faces. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either TopoDS_Face is null or vector
 * is empty.
 */
TopoDS_Shape Common(const TopoDS_Face& arguments,
                    const std::vector<TopoDS_Face>& tools);

//------------------------------------------------------------------------------

/**
 * Section between two lists of arguments.
 * Finds the section between positive and negative shapes.
 * positive represents the arguments, and negative represents the tools.
 *
 * @param positive A list of shapes to act as the main arguments for the
 * operation.
 * @param negative A list of shapes to act as the tools for the operation.
 * @return A shape representing the section between the two lists. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either list is empty.
 */
TopoDS_Shape Section(const TopTools_ListOfShape& positive,
                     const TopTools_ListOfShape& negative);

/**
 * Boolean section between two shapes.
 * Finds the section between the argument shape and the tool shape.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools The secondary shape to act as the tool for the operation.
 * @return A shape representing the section between the two shapes. Check if the
 * shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either shape is null.
 */
TopoDS_Shape Section(const TopoDS_Shape& arguments, const TopoDS_Shape& tools);

/**
 * Boolean section between a shape and a list of tools.
 * Finds the section between the argument shape and the list of tool shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools A list of shapes to act as the tools for the operation.
 * @return A shape representing the section between the argument shape and the
 * list of tools. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the argument shape is null or the list
 * of tools is empty.
 */
TopoDS_Shape Section(const TopoDS_Shape& arguments,
                     const TopTools_ListOfShape& tools);

/**
 * Boolean section between a shape and an initializer list of tools.
 * Finds the section between the argument shape and the initializer list of tool
 * shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools An initializer list of shapes to act as the tools for the
 * operation.
 * @return A shape representing the section between the argument shape and the
 * initializer list of tools. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the argument shape is null or the list
 * of tools is empty.
 */
TopoDS_Shape Section(const TopoDS_Shape& arguments,
                     const std::initializer_list<TopoDS_Shape>& tools);

/**
 * Boolean section between a shape and a variable shape container of arguments.
 * Finds the section between an argument shape and tool shapes.
 *
 * @param arguments The primary shape to act as the main argument for the
 * operation.
 * @param tools A container of shapes to act as the tools for the operation. The
 * container must be templated with a type and an allocator.
 * @return A shape representing the section between the input shape and the
 * container of shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either the shape is null or the
 * container is empty.
 */
template <template <typename, typename> typename Container, typename Allocator>
TopoDS_Shape Section(const TopoDS_Shape& arguments,
                     const Container<TopoDS_Shape, Allocator>& tools) {
  return Section(occutils::list_utils::ToOCCList({arguments}),
                 occutils::list_utils::ToOCCList(tools));
}

/**
 * Boolean section between two variable shape containers of arguments.
 * Finds the section between the argument and tool shapes.
 *
 * @param arguments A container of shapes to act as the main arguments for the
 * operation. The container must be templated with a type and an allocator.
 * @param tools A container of shapes to act as the tools for the operation. The
 * container must be templated with a type and an allocator.
 * @return A shape representing the section between the two containers of
 * shapes. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either container is empty.
 */
template <template <typename, typename> typename Container, typename Allocator1,
          typename Allocator2>
TopoDS_Shape Section(const Container<TopoDS_Shape, Allocator1>& arguments,
                     const Container<TopoDS_Shape, Allocator2>& tools) {
  return Section(occutils::list_utils::ToOCCList(arguments),
                 occutils::list_utils::ToOCCList(tools));
}

/**
 * Boolean section between two vectors of TopoDS_Solids.
 * Finds the section between the argument solids and the tool solids.
 *
 * @param arguments A vector of TopoDS_Solids to act as the main arguments for
 * the operation.
 * @param tools A vector of TopoDS_Solids to act as the tools for the operation.
 * @return A shape representing the section between the two vectors of solids.
 * Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Section(const std::vector<TopoDS_Solid>& arguments,
                     const std::vector<TopoDS_Solid>& tools);

/**
 * Boolean section between a TopoDS_Solid and a vector of TopoDS_Solids.
 * Finds the section between the argument solid and the vector of tool solids.
 *
 * @param arguments A TopoDS_Solid to act as the main argument for the
 * operation.
 * @param tools A vector of TopoDS_Solids to act as the tools for the operation.
 * @return A shape representing the section between the argument solid and the
 * vector of solids. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the argument solid is null or the
 * vector of tools is empty.
 */
TopoDS_Shape Section(const TopoDS_Solid& arguments,
                     const std::vector<TopoDS_Solid>& tools);

/**
 * Boolean section between two vectors of TopoDS_Face.
 * Finds the section between the argument faces and the tool faces.
 *
 * @param arguments A vector of TopoDS_Face to act as the main arguments for the
 * operation.
 * @param tools A vector of TopoDS_Face to act as the tools for the operation.
 * @return A shape representing the section between the two vectors of faces.
 * Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if either vector is empty.
 */
TopoDS_Shape Section(const std::vector<TopoDS_Face>& arguments,
                     const std::vector<TopoDS_Face>& tools);

/**
 * Boolean section between a TopoDS_Face and a vector of TopoDS_Faces.
 * Finds the section between the argument face and the vector of tool faces.
 *
 * @param arguments A TopoDS_Face to act as the main argument for the operation.
 * @param tools A vector of TopoDS_Faces to act as the tools for the operation.
 * @return A shape representing the section between the argument face and the
 * vector of faces. Check if the shape is null before using it.
 *
 * @throws OCCInvalidArgumentException if the argument face is null or the
 * vector of tools is empty.
 */
TopoDS_Shape Section(const TopoDS_Face& arguments,
                     const std::vector<TopoDS_Face>& tools);

}  // namespace occutils::boolean
