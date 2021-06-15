/**
 *  @file Utils.cpp
 *  @author Matteo Loporchio
 */

#include "Utils.hpp"
#include <cmath>

/**
 *
 */
bool lessThan(double a, double b, double eps) {
    return (b - a) > ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}

/**
 *
 */
bool approxEqual(double a, double b, double eps) {
    return fabs(a - b) <= ((fabs(a) < fabs(b) ? fabs(b) : fabs(a)) * eps);
}

/**
 *
 */
Record readRecord(const csv::CSVRow &row) {
  // Read report identifier.
  std::string report_id = row["REPORT_ID"].get<>();
  // Read location coordinates.
  uint64_t unique_loc = row["UNIQUE_LOC"].get<uint64_t>();
  double x = row["ACCLOC_X"].get<double>(),
  y = row["ACCLOC_Y"].get<double>();
  // Create and return the record.
  return {report_id, unique_loc, {x, y}};
}


/**
 *
 */
std::vector<Record> readFile(std::string &filename) {
  csv::CSVReader reader(filename);
  std::vector<Record> result;
  for (csv::CSVRow &row : reader) {
    try {
      Record e = readRecord(row);
      result.push_back(e);
    }
    catch (std::exception &e) {
      // Skip records with NaNs.
    }
  }
  return result;
}
