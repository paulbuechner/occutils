#pragma once

// std includes
#include <vector>

// OCC includes
#include <TopTools_ListOfShape.hxx>
#include <TopoDS_Compound.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Solid.hxx>

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
 * Create a compound from multiple faces
 */
TopoDS_Compound From(const std::vector<TopoDS_Face>& faces);

/**
 * Create a compound from multiple solids
 */
TopoDS_Compound From(const std::vector<TopoDS_Solid>& solids);

}  // namespace occutils::compound
