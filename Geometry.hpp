/**
 *  @file Geometry.hpp
 *  @author Matteo Loporchio
 */

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <cmath>

/**
 *  A point is represented by two integer coordinates.
 */
struct Point {
  int32_t x; ///< The x-coordinate of the point
  int32_t y; ///< The y-coordinate of the point
  /**
   *  "Strictly-less-than" operator to compare points lexicographically.
   *  @param p a point to be compared
   *  @return true if and only if the current point precedes p
   */
  bool operator<(const Point &p) const {
    return ((x != p.x) ? (x < p.x) : (y < p.y));
  }
};

/**
 *  A rectangle is represented by the coordinates of its lower-left
 *  and upper-right vertices.
 */
struct Rectangle {
  int32_t lx; ///< The x-coordinate of the lower-left vertex
  int32_t ly; ///< The y-coordinate of the lower-left vertex
  int32_t ux; ///< The x-coordinate of the upper-right vertex
  int32_t uy; ///< The y-coordinate of the upper-right vertex
};

/**
 *  The empty rectangle has vertices (+inf, +inf, -inf, -inf).
 */
#define EMPTY_RECT (Rectangle{INT_MAX, INT_MAX, INT_MIN, INT_MIN})

/**
 *  Returns true if and only if the given point is inside the given rectangle.
 *  @param r a rectangle
 *  @param p a point
 *  @return true if p is inside r, false otherwise
 */
static inline bool contains(const Rectangle &r, const Point &p) {
  return (r.lx <= p.x && p.x <= r.ux && r.ly <= p.y && p.y <= r.uy);
}

/**
 *  Returns true if and only if two rectangles overlap.
 *  @param r the first rectangle
 *  @param s the second rectangle
 *  @return true if r overlaps with s, false otherwise
 */
static inline bool intersect(const Rectangle &r, const Rectangle &s) {
  bool above = (r.ly >= s.uy), // r is above s
  below = (r.uy <= s.ly), // r is below s
  left = (r.ux <= s.lx), // r is to the left of s
  right = (r.lx >= s.ux); // r is to the right of s
  return !(above || below || left || right);
}

/**
 *  Computes the minimum bounding rectangle of a list of points.
 *  @param pts list of points
 *  @return the minimum bounding rectangle of the points
 */
static inline Rectangle mbr(const std::vector<Point> &pts) {
  Rectangle r = EMPTY_RECT;
  for (const Point &p : pts) {
    if (p.x <= r.lx) r.lx = p.x;
    if (p.y <= r.ly) r.ly = p.y;
    if (p.x >= r.ux) r.ux = p.x;
    if (p.y >= r.uy) r.uy = p.y;
  }
  return r;
}

/**
 *  Returns the rectangle with minimum area that encloses both
 *  the input rectangle and the input point.
 *  @param r rectangle
 *  @param p point
 *  @return the minimum bounding rectangle of input rectangle and point
 */
static inline Rectangle enlarge(const Rectangle &r, const Point &p) {
  return {
    std::min(r.lx, p.x), std::min(r.ly, p.y),
    std::max(r.ux, p.x), std::max(r.uy, p.y)
  };
}

/**
 *  Returns the rectangle with minimum area that encloses both rectangles.
 *  @param r first rectangle
 *  @param s second rectangle
 *  @return the minimum bounding rectangle of the input rectangles
 */
static inline Rectangle enlarge(const Rectangle &r, const Rectangle &s) {
  return {
    std::min(r.lx, s.lx), std::min(r.ly, s.ly),
    std::max(r.ux, s.ux), std::max(r.uy, s.uy)
  };
}


#endif
