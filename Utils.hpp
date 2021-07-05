/**
 *  @file Utils.hpp
 *  @author Matteo Loporchio
 */

#ifndef UTILS_H
#define UTILS_H

#include <vector>

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

#endif
