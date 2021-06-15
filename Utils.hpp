/**
 *  @file Utils.hpp
 *  @author Matteo Loporchio
 */

#ifndef UTILS_H
#define UTILS_H

#include "csv.hpp"
#include "Record.hpp"

/**
 *  Compares two floating point values and returns true if and only if
 *  the first one is strictly less than the second,
 *  according to the specified tolerance.
 *  @param a first value
 *  @param b second value
 *  @param eps floating-point tolerance
 *  @return true if and only if a is strictly less than b
 */
bool lessThan(double a, double b, double eps = 1E-12);

/**
 *  Compares two floating point values and returns true if and only if
 *  they are approximately equal, according to the specified tolerance.
 *  @param a first value
 *  @param b second value
 *  @param eps floating-point tolerance
 *  @return true if and only if a is approximately equal to b
 */
bool approxEqual(double a, double b, double eps = 1E-12);


/**
 *  Splits a vector into subvectors of k elements each.
 *  @param v input vector
 *  @param k size of each chunk
 *  @return a list of slices
 */
template<typename T>
std::vector<std::vector<T>> split(std::vector<T> &v, size_t k) {
  size_t i = 0;
  std::vector<std::vector<T>> result;
  while (i + k < v.size()) {
    result.push_back(std::vector<T>(v.begin() + i, v.begin() + i + k));
    i += k;
  }
  result.push_back(std::vector<T>(v.begin() + i, v.end()));
  return result;
}

#endif
