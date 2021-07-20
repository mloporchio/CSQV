/**
 *  @file Config.hpp
 *  @author Matteo Loporchio
 */

#ifndef CONFIG_H
#define CONFIG_H

/**
 *  Comment if you want to sort points according to the lexicographical order
 *  and not on their Morton index.
 */
#define Z_INDEX

/**
 *  Uncomment if you want to print to a file the MBRs stored at the root
 *  after the MR-tree index construction.
 */
//#define LOG_RECT

/**
 *  This is the path of the file containing the MBRs stored at the root.
 */
//#define LOG_RECT_FILE "rectangles.csv"

#endif
