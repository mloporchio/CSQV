/**
 *  @file Record.hpp
 *  @author Matteo Loporchio
 */

#ifndef RECORD_H
#define RECORD_H

#include "Geometry.hpp"

struct Record {
  std::string report_id;
  uint64_t unique_loc; // unique location
  Point loc; // location
};

struct RecordCmp {
  bool operator()(const Record &a, const Record &b) {
    return a.loc < b.loc;
  }
};

#endif
