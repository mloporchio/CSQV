/**
 *  @file Record.cpp
 *  @author Matteo Loporchio
 */

#include "csv.hpp"
#include "Record.hpp"


/**
 *  "Strictly-less-than" operator to compare records.
 *  @param e a record to be compared with the current one
 *  @return true if and only if the current record is strictly less
 *  than the input one
 */
bool Record::operator<(const Record &e) const {
  return (loc < e.loc);
}

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
    std::string report_id = row["REPORT_ID"].get<>();
    // Read information about year, month, day and time.
    uint32_t year = row["Year"].get<uint32_t>();
    std::string month = row["Month"].get<>();
    std::string day = row["Day"].get<>();
    std::string time = row["Time"].get<>();
    // Read location coordinates.
    double x = row["ACCLOC_X"].get<double>(),
    y = row["ACCLOC_Y"].get<double>();
    // Create the record and add it to the list.
    result.push_back({report_id, year, month, day, time, {x, y}});
  }
  return result;
}

/**
 *  Inserts all the fields of a record into a given buffer.
 *  @param buf the buffer
 *  @param r the record
 */
void put_record(Buffer &buf, Record &r) {
  buf.put_bytes((uint8_t*) r.report_id.c_str(), r.report_id.size())
  .put(r.year)
  .put_bytes((uint8_t*) r.month.c_str(), r.month.size())
  .put_bytes((uint8_t*) r.day.c_str(), r.day.size())
  .put_bytes((uint8_t*) r.time.c_str(), r.day.size())
  .put(r.loc.x).put(r.loc.y);
}

/**
 *  This function computes a SHA-256 digest of a record
 *  by concatenating all raw bytes of its fields.
 *  @param r input record
 *  @return a digest of the record
 */
hash_t hash_record(Record &r) {
  Buffer buf;
  buf.put_bytes((uint8_t*) r.report_id.c_str(), r.report_id.size())
  .put(r.year)
  .put_bytes((uint8_t*) r.month.c_str(), r.month.size())
  .put_bytes((uint8_t*) r.day.c_str(), r.day.size())
  .put_bytes((uint8_t*) r.time.c_str(), r.day.size())
  .put(r.loc.x).put(r.loc.y);
  return sha256(buf);
}
