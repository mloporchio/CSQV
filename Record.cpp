/**
 *  @file Record.cpp
 *  @author Matteo Loporchio
 */

#include "csv.hpp"
#include "Record.hpp"
#ifdef Z_INDEX
#include "libmorton/morton.h"
#endif

/**
 *  Parses a CSV file and creates a list of records.
 *  @param path full path of the input file
 *  @return a list of records parsed from the input file
 */
std::vector<Record> load_file(const std::string &path) {
  csv::CSVReader reader(path);
  std::vector<Record> result;
  for (csv::CSVRow &row : reader) {
    // Read report identifier.
    std::string report_id = row["ID"].get<>();
    // Read information about year, month, day and time.
    uint32_t year = row["Year"].get<uint32_t>();
    std::string month = row["Month"].get<>();
    std::string day = row["Day"].get<>();
    std::string time = row["Time"].get<>();
    // Read the coordinates and compute their Morton index.
    int32_t x = row["x"].get<int32_t>(), y = row["y"].get<int32_t>();
    #ifdef Z_INDEX
    uint_fast64_t z_index =
    libmorton::morton2D_64_encode((uint_fast32_t) x, (uint_fast32_t) y);
    result.push_back({report_id, year, month, day, time, {x, y}, z_index});
    #else
    result.push_back({report_id, year, month, day, time, {x, y}});
    #endif
  }
  return result;
}
