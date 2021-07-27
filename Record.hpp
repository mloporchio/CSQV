/**
 *  @file Record.hpp
 *  @author Matteo Loporchio
 */

#ifndef RECORD_H
#define RECORD_H

#include "Config.hpp"
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
  #ifdef Z_INDEX
  uint_fast64_t z_index; ///< Morton index of the location
  #endif
};

/**
 *  Record comparison function.
 *  @param e1 the first record
 *  @param e2 the second record
 *  @return true if and only if the first record precedes the second
 */
static inline bool record_cmp(const Record &e1, const Record &e2) {
  #ifdef Z_INDEX
  return e1.z_index < e2.z_index;
  #else
  return e1.loc < e2.loc;
  #endif
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
 *  Given a list, counts the number of records that match a query.
 *  @param data list of records
 *  @param q the query rectangle
 *  @return the number of matching records
 */
static inline size_t count(const std::vector<Record> &data,
  const Rectangle &q) {
  size_t count = 0;
  for (const Record &e : data) if (match(e, q)) count++;
  return count;
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
static inline void put_record(Buffer &buf, const Record &r) {
  buf.put_bytes((uint8_t*) r.report_id.c_str(), r.report_id.size())
  .put(r.year)
  .put_bytes((uint8_t*) r.month.c_str(), r.month.size())
  .put_bytes((uint8_t*) r.day.c_str(), r.day.size())
  .put_bytes((uint8_t*) r.time.c_str(), r.day.size())
  .put(r.loc.x).put(r.loc.y);
}

#endif
