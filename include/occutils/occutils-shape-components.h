#pragma once

/**
 * Utilities for dissecting shapes into their components.
 */

// std includes
#include <optional>
#include <vector>

// OCC includes
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Wire.hxx>

namespace occutils::shape_components {

/**
 * Iterate over all components of shape and count how many
 * sub-shapes of type [type] it contains.
 *
 * NOTE: shape itself will NOT count, even if it is of type [type]
 */
size_t CountX(const TopoDS_Shape& shape, TopAbs_ShapeEnum type);
size_t CountX(const std::vector<TopoDS_Shape>& shapes, TopAbs_ShapeEnum type);
size_t CountSolids(const TopoDS_Shape& shape, TopAbs_ShapeEnum type);

//------------------------------------------------------------------------------

/**
 * Get all solids in a given shape
 * (not including the shape itself, if it is a solid)
 */
std::vector<TopoDS_Solid> AllSolidsWithin(const TopoDS_Shape& shape);

/**
 * Get all solids in the given shapes
 * (not including the shapes itself, if it is a solid)
 */
std::vector<TopoDS_Solid> AllSolidsWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * Get all shells in a given shape
 * (not including the shape itself, if it is a shell)
 */
std::vector<TopoDS_Shell> AllShellsWithin(const TopoDS_Shape& shape);

/**
 * Get all shells in the given shapes
 * (not including the shapes itself, if it is a shell)
 */
std::vector<TopoDS_Shell> AllShellsWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * Get all faces in a given shape
 * (not including the shape itself, if it is a face)
 */
std::vector<TopoDS_Face> AllFacesWithin(const TopoDS_Shape& shape);

/**
 * Get all faces in the given shapes
 * (not including the shapes itself, if it is a face)
 */
std::vector<TopoDS_Face> AllFacesWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * Get all edges in a given shape
 * (not including the shape itself, if it is an edge)
 */
std::vector<TopoDS_Edge> AllEdgesWithin(const TopoDS_Shape& shape);

/**
 * Get all edges in the given shapes
 * (not including the shapes itself, if it is a edge)
 */
std::vector<TopoDS_Edge> AllEdgesWithin(
    const std::vector<TopoDS_Shape>& shapes);

/**
 * Get all edges in the given shapes
 * (not including the shapes itself, if it is a edge)
 */
std::vector<TopoDS_Edge> AllEdgesWithin(const std::vector<TopoDS_Wire>& wires);

//------------------------------------------------------------------------------

/**
 * Get all wires in a given shape
 * (not including the shape itself, if it is a wire)
 */
std::vector<TopoDS_Wire> AllWiresWithin(const TopoDS_Shape& shape);

/**
 * Get all wires in the given shapes
 * (not including the shapes itself, if it is a wire)
 */
std::vector<TopoDS_Wire> AllWiresWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * Get all vertices in a given shape
 * (not including the shape itself, if it is a vertex)
 */
std::vector<TopoDS_Vertex> AllVerticesWithin(const TopoDS_Shape& shape);

/**
 * Get all vertices in the given shapes
 * (not including the shapes itself, if it is a vertex)
 */
std::vector<TopoDS_Vertex> AllVerticesWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * Get all vertex coordinates in a given shape.
 * Like AllVerticesWithin() but converts the TopoDS_Vertex instances
 * to gp_Pnts (not including the shape itself, if it is a vertex)
 */
std::vector<gp_Pnt> AllVertexCoordinatesWithin(const TopoDS_Shape& shape);

/**
 * Get all vertices coordinates in the given shapes.
 * Like AllVerticesWithin() but converts the TopoDS_Vertex instances
 * to gp_Pnts (not including the shape itself, if it is a vertex)
 */
std::vector<gp_Pnt> AllVertexCoordinatesWithin(
    const std::vector<TopoDS_Shape>& shapes);

//------------------------------------------------------------------------------

/**
 * If [shape] is a solid, return shape.
 * Else, if there is a single solid within [shape],
 * returns that solid. Else, returns no value.
 *
 * If there are multiple solids within shape:
 *  - if [firstOfMultipleOK] == true => get first of these solids
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Solid> TryGetSingleSolid(const TopoDS_Shape& shape,
                                              bool firstOfMultipleOK = true);

/**
 * If [shape] is a shell, return shape.
 * Else, if there is a single shell within [shape],
 * returns that shell. Else, returns no value.
 *
 * If there are multiple shells within shape:
 *  - if [firstOfMultipleOK] == true => get first of these shells
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Shell> TryGetSingleShell(const TopoDS_Shape& shape,
                                              bool firstOfMultipleOK = true);

/**
 * If [shape] is a face, return shape.
 * Else, if there is a single face within [shape],
 * returns that face. Else, returns no value.
 *
 * If there are multiple faces within shape:
 *  - if [firstOfMultipleOK] == true => get first of these faces
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Face> TryGetSingleFace(const TopoDS_Shape& shape,
                                            bool firstOfMultipleOK = true);

/**
 * If [shape] is a edge, return shape.
 * Else, if there is a single edge within [shape],
 * returns that edge. Else, returns no value.
 *
 * If there are multiple edges within shape:
 *  - if [firstOfMultipleOK] == true => get first of these edges
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Edge> TryGetSingleEdge(const TopoDS_Shape& shape,
                                            bool firstOfMultipleOK = true);

/**
 * If [shape] is a edge, return shape.
 * Else, if there is a single edge within [shape],
 * returns that edge. Else, returns no value.
 *
 * If there are multiple wires within shape:
 *  - if [firstOfMultipleOK] == true => get first of these wires
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Wire> TryGetSingleWire(const TopoDS_Shape& shape,
                                            bool firstOfMultipleOK = true);

/**
 * If [shape] is a vertex, return shape.
 * Else, if there is a single vertex within [shape],
 * returns that vertex. Else, returns no value.
 *
 * If there are multiple vertices within shape:
 *  - if [firstOfMultipleOK] == true => get first of these vertices
 *  - if [firstOfMultipleOK] == false => return std::nullopt
 */
std::optional<TopoDS_Vertex> TryGetSingleVertex(const TopoDS_Shape& shape,
                                                bool firstOfMultipleOK = true);

//------------------------------------------------------------------------------

/**
 * If [shape] is a solid, return shape.
 * Else, expects there to be a single solid within [shape].
 * If there are zero solids within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ solids within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Solid GetSingleSolid(const TopoDS_Shape& shape,
                            bool firstOfMultipleOK = true);

/**
 * If [shape] is a shell, return shape.
 * Else, expects there to be a single shell within [shape].
 * If there are zero shells within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ shells within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Shell GetSingleShell(const TopoDS_Shape& shape,
                            bool firstOfMultipleOK = true);

/**
 * If [shape] is a face, return shape.
 * Else, expects there to be a single face within [shape].
 * If there are zero faces within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ faces within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Face GetSingleFace(const TopoDS_Shape& shape,
                          bool firstOfMultipleOK = true);

/**
 * If [shape] is an edge, return shape.
 * Else, expects there to be a single edge within [shape].
 * If there are zero edges within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ edges within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Edge GetSingleEdge(const TopoDS_Shape& shape,
                          bool firstOfMultipleOK = true);

/**
 * If [shape] is a wire, return shape.
 * Else, expects there to be a single wire within [shape].
 * If there are zero wires within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ wires within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Wire GetSingleWire(const TopoDS_Shape& shape,
                          bool firstOfMultipleOK = true);

/**
 * If [shape] is an vertex, return shape.
 * Else, expects there to be a single vertex within [shape].
 * If there are zero vertices within [shape], throws
 * OCCTopologyCountMismatchException(). If there are 2+ vertices within [shape]:
 *  - if [firstOfMultipleOK] == true => get first shape
 *  - if [firstOfMultipleOK] == false => throws
 * OCCTopologyCountMismatchException().
 */
TopoDS_Vertex GetSingleVertex(const TopoDS_Shape& shape,
                              bool firstOfMultipleOK = true);

}  // namespace occutils::shape_components
