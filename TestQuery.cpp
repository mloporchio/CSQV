/**
 *  @file TestQuery.cpp
 *  @author Matteo Loporchio
 *
 *  This program can be used to test the MR-tree query and verification
 *  algorithms. To execute it, the following parameters are required:
 *
 *    <filename> <capacity> <xmin> <ymin> <xmax> <ymax>
 */

#include "Query.hpp"
#include <cstdlib>
#include <iostream>

using namespace std::chrono;

int main(int argc, char const *argv[]) {
  // Check the number of input arguments.
  if (argc < 7) {
    std::cerr << "Usage: " << argv[0]
    << " <filename> <capacity> <xmin> <ymin> <xmax> <ymax>" << std::endl;
    return 1;
  }
  // Read the parameters.
  std::string filename = argv[1];
  size_t capacity = atol(argv[2]);
  double xmin = atof(argv[3]), ymin = atof(argv[4]),
  xmax = atof(argv[5]), ymax = atof(argv[6]);
  Rectangle query_rect = {xmin, ymin, xmax, ymax};
  // Open the input file and construct the MR-tree index.
  std::vector<Record> records = load_file(filename);
  Node *root = packed(records, capacity);
  // Time the execution of the query algorithm.
  auto q_start = high_resolution_clock::now();
  VObject *vo = query_it(root, query_rect);
  auto q_stop = high_resolution_clock::now();
  auto q_duration = duration_cast<microseconds>(q_stop - q_start);
  // Time the execution of the verification algorithm.
  auto v_start = high_resolution_clock::now();
  VResult *vr = verify_it(vo, query_rect);
  auto v_stop = high_resolution_clock::now();
  auto v_duration = duration_cast<microseconds>(v_stop - v_start);
  // Print information.
  std::cout << "Hash: " << toHex(root->getHash()) << std::endl
  << "Reconstructed hash: " << toHex(vr->getHash()) << std::endl
  << "Query time (us): " << q_duration.count() << std::endl
  << "Verification time (us): " << v_duration.count() << std::endl;
  // Before closing, free the memory occupied by the index.
  delete_tree(root);
  return 0;
}
