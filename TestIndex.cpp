/**
 *  @file TestIndex.cpp
 *  @author Matteo Loporchio
 *
 *  This program can be used to test the construction
 *  of a MR-tree index from a set of records.
 *  In order to run it, the following parameters are required:
 *
 *    <filename> <capacity>
 */

#include "Node.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>

using namespace std::chrono;

int main(int argc, char **argv) {
  // Check the number of input arguments.
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] << " <filename> <capacity>" << std::endl;
    return 1;
  }
  std::string filename = argv[1];
  size_t capacity = atol(argv[2]);
  // Open the input file and parse its content.
  std::vector<Record> records = load_file(filename);
  // Build the MR-tree index.
  auto start = high_resolution_clock::now();
  Node *root = packed(records, capacity);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  // Print the results.
  std::cout
  << "Construction time (us): " << duration.count() << std::endl
  << "Leaves: " << count_leaves(root) << std::endl
  << "Height: " << height(root) << std::endl;
  // Free the memory occupied by the index.
  delete_tree(root);
  return 0;
}
