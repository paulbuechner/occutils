#pragma once

// std includes
#include <initializer_list>
#include <vector>

// OCC includes
#include <Precision.hxx>
#include <gp_Pnt.hxx>

/**
 * Add the coordinates of two points.
 * Performs coordinate-wise addition.
 */
gp_Pnt operator+(const gp_Pnt &a, const gp_Pnt &b);
gp_Pnt operator+(const gp_Pnt &a, const gp_Vec &b);
gp_Pnt operator+(const gp_Pnt &a, const gp_XYZ &b);

/**
 * Subtract the coordinates of two points.
 * Performs coordinate-wise subtraction.
 */
gp_Pnt operator-(const gp_Pnt &a, const gp_Pnt &b);
gp_Pnt operator-(const gp_Pnt &a, const gp_Vec &b);
gp_Pnt operator-(const gp_Pnt &a, const gp_XYZ &b);

namespace occutils::point {

/**
 * Return the global coordinate system's origin:
 * (0,0,0)
 */
gp_Pnt Origin();

/**
 * Generate a 3D point from a 2D point
 * by setting the Z coordinate to 0.0
 */
gp_Pnt From2d(const gp_Pnt2d &pnt);
gp_Pnt From2d(double x, double y);

/**
 * Get the point of mean X/Y/Z between the given points
 */
gp_Pnt Midpoint(const std::initializer_list<gp_Pnt> &points);
gp_Pnt Midpoint(const std::vector<gp_Pnt> &points);

/**
 * Get the distance between pnt and axis,
 * strictly defined as the distance between pnt and the orthogonal
 * projection of pnt onto axis.
 */
double Distance(const gp_Pnt &pnt, const gp_Ax1 &axis);

/**
 * Orthogonally project pnt onto ax
 */
gp_Pnt OrthogonalProjectOnto(const gp_Pnt &pnt, const gp_Ax1 &ax);

/**
 * Orthogonally project pnt onto ax
 */
gp_Pnt2d OrthogonalProjectOnto(const gp_Pnt2d &pnt, const gp_Ax2d &ax);

/**
 * @struct Compare
 * @brief Comparator for gp_Pnt objects.
 *
 * This struct provides a comparison operator for gp_Pnt objects, which compares
 * the points based on their X, Y, and Z coordinates with a precision threshold.
 */
struct Compare {
  /**
   * @brief The tolerance for comparing two points.
   */
  double tol;

  /**
   * @brief Construct a new Compare object for comparing gp_Pnt.
   *
   * @param tol The tolerance for comparing two points, defaults to
   * Precision::Confusion() = 1e-7.
   */
  explicit Compare(const double tol = Precision::Confusion()) : tol(tol) {}

  /**
   * @brief Compare two gp_Pnt objects.
   *
   * This operator compares two gp_Pnt objects based on their X, Y, and Z
   * coordinates. The comparison is done with a precision threshold
   * (Precision::Confusion() = 1e-7) to handle floating-point inaccuracies.
   *
   * @param a The first gp_Pnt object to compare.
   * @param b The second gp_Pnt object to compare.
   * @return True if the first point is considered less than the second point,
   * false otherwise.
   */
  bool operator()(const gp_Pnt &a, const gp_Pnt &b) const {
    if (std::abs(a.X() - b.X()) > tol) return a.X() < b.X();
    if (std::abs(a.Y() - b.Y()) > tol) return a.Y() < b.Y();
    if (std::abs(a.Z() - b.Z()) > tol) return a.Z() < b.Z();
    return false;
  }
};

}  // namespace occutils::point
