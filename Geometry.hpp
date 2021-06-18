/**
 *  @file Geometry.hpp
 *  @author Matteo Loporchio
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>

/**
 *  A point is represented by two real coordinates.
 */
struct Point {
  double x; ///< The x-coordinate of the point
  double y; ///< The y-coordinate of the point

  /**
   *  "Strictly-less-than" comparison operator that can be used
   *  to sort a list of points.
   *  @param p a point to be compared
   *  @return true if and only if the current point is strictly less than
   *  the input point
   */
  bool operator<(const Point &p) const;
};

/**
 *  A rectangle is represented by the coordinates of its lower-left
 *  and upper-right vertices.
 */
struct Rectangle {
  double lx; ///< The x-coordinate of the lower-left vertex
  double ly; ///< The y-coordinate of the lower-left vertex
  double ux; ///< The x-coordinate of the upper-right vertex
  double uy; ///< The y-coordinate of the upper-right vertex
};

/**
 *  Returns true if and only if the given point is inside the given rectangle.
 *  @param r a rectangle
 *  @param p a point
 *  @return true if p is inside r, false otherwise
 */
bool contains(const Rectangle &r, const Point &p);

/**
 *  Returns true if and only if two rectangles overlap.
 *  @param r the first rectangle
 *  @param s the second rectangle
 *  @return true if r overlaps with s, false otherwise
 */
bool intersect(const Rectangle &r, const Rectangle &s);

/**
 *  Returns an empty rectangle with vertices (+inf, +inf, -inf, -inf).
 *  @return an empty rectangle
 */
Rectangle empty();

/**
 *  Computes the minimum bounding rectangle of a list of points.
 *  @param pts list of points
 *  @return the minimum bounding rectangle of the points
 */
Rectangle mbr(const std::vector<Point> &pts);

/**
 *  Returns the rectangle with minimum area that encloses both
 *  the input rectangle and the input point.
 *  @param r rectangle
 *  @param p point
 *  @return the minimum bounding rectangle of input rectangle and point
 */
Rectangle enlarge(const Rectangle &r, const Point &p);

/**
 *  Returns the rectangle with minimum area that encloses both rectangles.
 *  @param r first rectangle
 *  @param s second rectangle
 *  @return the minimum bounding rectangle of the input rectangles
 */
Rectangle enlarge(const Rectangle &r, const Rectangle &s);


#endif
