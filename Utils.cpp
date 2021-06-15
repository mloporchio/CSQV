/**
 *  @file Utils.cpp
 *  @author Matteo Loporchio
 */

#include "Utils.hpp"
#include <cmath>

/**
 *  Compares two floating point values and returns true if and only if
 *  the first one is strictly less than the second,
 *  according to the specified tolerance.
 *  @param a first value
 *  @param b second value
 *  @param eps floating-point tolerance
 *  @return true if and only if a is strictly less than b
 */
bool lessThan(double a, double b, double eps) {
    return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}

/**
 *  Compares two floating point values and returns true if and only if
 *  they are approximately equal, according to the specified tolerance.
 *  @param a first value
 *  @param b second value
 *  @param eps floating-point tolerance
 *  @return true if and only if a is approximately equal to b
 */
bool approxEqual(double a, double b, double eps) {
    return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}
