/**
 *  @file TestSize.cpp
 *  @author Matteo Loporchio
 *
 *  This program returns the average approximate size (in bytes)
 *  of the records in a given data set.
 *  In order to run it, the following parameters are required:
 *
 *    <filename>
 */

#include "Record.hpp"
#include <iostream>

int main(int argc, char const *argv[]) {
  // Check the number of input arguments.
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  // Open the input file and parse its content.
  std::vector<Record> records = load_file(filename);
  // Scan the list and compute the average size.
  size_t avg_size = 0;
  for (Record &e : records) avg_size += size(e);
  avg_size /= records.size();
  std::cout << "N. of records: " << records.size() << std::endl
  << "Average size (bytes): " << avg_size << std::endl;
  return 0;
}
