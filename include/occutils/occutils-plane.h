#pragma once

// OCC includes
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>

namespace occutils::plane {

/**
 * Construct an infinite plane from three points.
 * The plane's cartesian coordinate system is:
 *  - (U,V)(0,0) = pO
 *  - X axis = gp_Vec(pO, pX)
 *  - Y axis = gp_Vec(pO, pY)
 * Precondition: No pair of points must coincide.
 * @throws OCCConstructionFailedException() if the parameters are invalid
 */
gp_Pln FromPoints(const gp_Pnt& pO, const gp_Pnt& pX, const gp_Pnt& pY);

}  // namespace occutils::plane
