#pragma once

/**
 * Utilities for dissecting shapes into their components.
 */

// std includes
#include <vector>

// OCC includes
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

namespace occutils {

namespace shape {

/**
 * @return true if the given shape is a TopoDS_CompSolid
 */
bool IsCompoundSolid(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Compound
 */
bool IsCompound(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Solid
 */
bool IsSolid(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Shell
 */
bool IsShell(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Face
 */
bool IsFace(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Wire
 */
bool IsWire(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Edge
 */
bool IsEdge(const TopoDS_Shape& shape);

/**
 * @return true if the given shape is a TopoDS_Vertex
 */
bool IsVertex(const TopoDS_Shape& shape);

/**
 * Compute the volume of the given shape.
 */
double Volume(const TopoDS_Shape& shape);

}  // namespace shape

namespace shapes {

/**
 * Convert a solid vector to a shape vector
 */
std::vector<TopoDS_Shape> FromSolids(const std::vector<TopoDS_Solid>& solids);

/**
 * Convert a face vector to a shape vector
 */
std::vector<TopoDS_Shape> FromFaces(const std::vector<TopoDS_Face>& faces);

}  // namespace shapes

}  // namespace occutils
