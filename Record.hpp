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
 *  Parses a CSV file and creates a list of records.
 *  @param path full path of the input file
 *  @return a list of records parsed from the input file
 */
std::vector<Record> load_file(const std::string &path);

/**
 *  This function computes a SHA-256 digest of a record
 *  by concatenating all raw bytes of its fields.
 *  @param r input record
 *  @return a digest of the record
 */
hash_t hash_record(Record &r);

#endif
