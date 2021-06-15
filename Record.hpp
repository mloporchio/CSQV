/**
 *  @file Record.hpp
 *  @author Matteo Loporchio
 */

#ifndef RECORD_H
#define RECORD_H

#include "Geometry.hpp"
#include "Hash.hpp"

/**
 *
 */
struct Record {
  std::string report_id;
  uint32_t year;
  std::string month;
  std::string day;
  std::string time;
  Point loc;
};

/**
 *  Comparator for sorting a list of records.
 *  Elements are sorted according to their location.
 */
struct RecordCmp {
  bool operator()(const Record &a, const Record &b) {
    return a.loc < b.loc;
  }
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
