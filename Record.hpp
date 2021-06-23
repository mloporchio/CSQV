/**
 *  @file Record.hpp
 *  @author Matteo Loporchio
 */

#ifndef RECORD_H
#define RECORD_H

#include "Geometry.hpp"
#include "Hash.hpp"

/**
 *  A record represents a car crash event.
 */
struct Record {
  std::string report_id; ///< The identifier of the police report
  uint32_t year; ///< Year of the event
  std::string month; ///< Month of the event
  std::string day; ///< Day of the event
  std::string time; ///< Time of the event
  Point loc; ///< The geographical location of the event

  /**
   *  "Strictly-less-than" operator to compare records.
   *  @param e a record to be compared with the current one
   *  @return true if and only if the current record is strictly less
   *  than the input one
   */
  bool operator<(const Record &e) const;
};

/**
 *  Returns (an approximation of) the memory occupied by a record (in bytes).
 *  @param e a record
 *  @return an approximation of the record size
 */
static inline size_t size(const Record &e) {
  return (e.report_id.size() + e.month.size() + e.day.size()
  + e.time.size() + sizeof(uint32_t) + 2*sizeof(double));
}

/**
 *  Checks if a given record matches a query.
 *  @param e the record
 *  @param q the query rectangle
 *  @return true if and only if the record satisfies the query
 */
static inline bool match(const Record &e, const Rectangle &q) {
  return (q.lx <= e.loc.x && e.loc.x <= q.ux &&
          q.ly <= e.loc.y && e.loc.y <= q.uy);
}

/**
 *  Parses a CSV file and creates a list of records.
 *  @param path full path of the input file
 *  @return a list of records parsed from the input file
 */
std::vector<Record> load_file(const std::string &path);


/**
 *  Inserts all the fields of a record into a given buffer.
 *  @param buf the buffer
 *  @param r the record
 */
static inline void put_record(Buffer &buf, Record &r) {
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
hash_t hash_record(Record &r);

#endif
