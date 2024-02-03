#pragma once

// std includes
#include <optional>
#include <vector>

// OCC includes
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <gp_Pnt.hxx>

// occutils includes
#include "occutils/occutils-direction.h"

namespace occutils::wire {

/**
 * Create a wire from a single edge. Ensure that the edge is a valid candidate
 * to create a wire from, i.e. its curve should be closed.
 *
 * @param edge The edge to create the wire from
 * @return The wire created from the edge
 */
TopoDS_Wire FromEdge(const TopoDS_Edge& edge);

/**
 * Create a wire from one or multiple edges.
 * Convenience function to be used like this:
 *  wire::FromEdges
 */
TopoDS_Wire FromEdges(const std::initializer_list<TopoDS_Edge>& edges);
TopoDS_Wire FromEdges(const std::vector<TopoDS_Edge>& edges);

/**
 * Make a TopoDS_Wire by linearly connecting all of the given points in order,
 * @param close If set to true, connect the last point to the first point.
 */
TopoDS_Wire FromPoints(const std::vector<gp_Pnt>& edges, bool close = false);

/**
 * Build a wire incrementally,
 * uses relative coordinates.
 *
 * This is a convenience wrapper to
 * programmatically build wires.
 */
class IncrementalBuilder {
 public:
  IncrementalBuilder(const gp_Pnt& pnt);

  /**
   * Add a line segment
   */
  void Line(double dx, double dy, double dz);

  /**
   * ALPHA - API MAY CHANGE!
   * Create a 90° arc from the current position.
   */
  void Arc90(double dx, double dy, double dz, double centerDx, double centerDy,
             double centerDz, const gp_Dir& normal = occutils::direction::Z());

  /**
   * Get the current direction vector,
   * i.e. the end direction of the resulting edge.
   */
  std::optional<gp_Dir> Direction();

  /**
   * Get the current position
   */
  gp_Pnt Location();

  /**
   * Get the resulting wire.
   */
  [[nodiscard]] TopoDS_Wire Wire() const;

  /**
   * Create a pipe from the wire using the given profile.
   */
  [[nodiscard]] TopoDS_Shape Pipe(const TopoDS_Face& profile) const;

  /**
   * Create a pipe from the wire using a circular profile
   * of the given radius.
   */
  TopoDS_Shape PipeWithCircularProfile(double radius);

  // Current location
  gp_Pnt current;

  // Current direction
  std::optional<gp_Dir> currentDirection;
  std::vector<TopoDS_Edge> edges;
};

}  // namespace occutils::wire
