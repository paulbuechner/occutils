#include "occutils/occutils-plane.h"

// OCC includes
#include <Precision.hxx>
#include <gp_Pln.hxx>
#include <gp_Pnt.hxx>
#include <gp_Vec.hxx>

// occutils includes
#include "occutils/occutils-equality.h"
#include "occutils/occutils-exceptions.h"

namespace occutils::plane
{

gp_Pln FromPoints(const gp_Pnt& pO, const gp_Pnt& pX, const gp_Pnt& pY)
{
  if (pO == pX)
  {
    throw OCCConstructionFailedException("Plane construction failed: pO coincides with pX");
  }

  if (pO == pY)
  {
    throw OCCConstructionFailedException("Plane construction failed: pO coincides with pY");
  }

  if (pX == pY)
  {
    throw OCCConstructionFailedException("Plane construction failed: pX coincides with pY");
  }
  
  const gp_Dir xAx(gp_Vec(pO, pX));
  const gp_Dir yAx(gp_Vec(pO, pY));
  if (!xAx.IsNormal(yAx, Precision::Angular()))
  {
    throw OCCConstructionFailedException(
      "Plane construction failed: Axes are not normal to each other");
  }
  // Ax3 takes normal axis
  const gp_Dir normal = xAx.Crossed(yAx);
  return {gp_Ax3(pO, normal, xAx)};
}

} // namespace occutils::plane
