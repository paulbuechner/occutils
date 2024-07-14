#pragma once

// std includes
#include <vector>

// OCC includes
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_CompSolid.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Shell.hxx>
#include <TopoDS_Solid.hxx>
#include <TopoDS_Vertex.hxx>

namespace occutils::compound {

/**
 * Create a compound from multiple shapes
 */
TopoDS_Compound From(const TopTools_ListOfShape& shapes);

/**
 * Create a compound from multiple shapes
 */
TopoDS_Compound From(const std::vector<TopoDS_Shape>& shapes);

/**
 * Create a compound from multiple compounds
 */
TopoDS_Compound From(const std::vector<TopoDS_Compound>& comps);

/**
 * Create a compound from multiple compound-solids
 */
TopoDS_Compound From(const std::vector<TopoDS_CompSolid>& compSolids);

/**
 * Create a compound from multiple solids
 */
TopoDS_Compound From(const std::vector<TopoDS_Solid>& solids);

/**
 * Create a compound from multiple shells
 */
TopoDS_Compound From(const std::vector<TopoDS_Shell>& shells);

/**
 * Create a compound from multiple faces
 */
TopoDS_Compound From(const std::vector<TopoDS_Face>& faces);

/**
 * Create a compound from multiple wires
 */
TopoDS_Compound From(const std::vector<TopoDS_Wire>& wires);

/**
 * Create a compound from multiple edges
 */
TopoDS_Compound From(const std::vector<TopoDS_Edge>& edges);

/**
 * Create a compound from multiple vertices
 */
TopoDS_Compound From(const std::vector<TopoDS_Vertex>& vertices);

}  // namespace occutils::compound
