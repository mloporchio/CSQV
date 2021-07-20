/**
 *  @file TestQuery.cpp
 *  @author Matteo Loporchio
 *
 *  This program can be used to test the MR-tree query and verification
 *  algorithms. To execute it, the following parameters are required:
 *
 *    TestQuery <data_file> <query_file> <capacity>
 */

#include "csv.hpp"
#include "Query.hpp"
#include <cstdlib>
#include <iostream>

using namespace std::chrono;

/**
 *  Reads a list of query rectangles from a given file.
 *  @param path the full path of the input file
 *  @return a list of rectangles
 */
std::vector<Rectangle> load_query_file(const std::string &path) {
  csv::CSVReader reader(path);
  std::vector<Rectangle> result;
  for (csv::CSVRow &row : reader) {
    // Read the coordinates
    int32_t lx = row["lx"].get<int32_t>(), ly = row["ly"].get<int32_t>(),
    ux = row["ux"].get<int32_t>(), uy = row["uy"].get<int32_t>();
    // Create the rectangle.
    result.push_back({lx, ly, ux, uy});
  }
  return result;
}

int main(int argc, char const *argv[]) {
  // Check the number of input arguments.
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0]
    << " <data_file> <query_file> <capacity>" << std::endl;
    return 1;
  }
  // Read the parameters.
  std::string data_file = argv[1], query_file = argv[2];
  size_t capacity = atol(argv[3]);
  // Open the input file and construct the MR-tree index.
  std::vector<Record> records = load_file(data_file);
  Node *root = packed(records, capacity);
  // Read the query file.
  std::vector<Rectangle> queries = load_query_file(query_file);
  // Execute the queries.
  VObject *vo = NULL;
  VResult *vr = NULL;
  long long q_time = 0, v_time = 0;
  size_t returned = 0, matching = 0;
  for (size_t i = 0; i < queries.size(); i++) {
    // Time the execution of the query algorithm.
    auto q_start = high_resolution_clock::now();
    vo = query_it(root, queries[i]);
    auto q_stop = high_resolution_clock::now();
    // Time the execution of the verification algorithm.
    auto v_start = high_resolution_clock::now();
    vr = verify_it(vo, queries[i]);
    auto v_stop = high_resolution_clock::now();
    // Update the values for statistics.
    returned += count_records(vo);
    matching += vr->count();
    q_time += (duration_cast<microseconds>(q_stop - q_start)).count();
    v_time += (duration_cast<microseconds>(v_stop - v_start)).count();
  }
  // Print information.
  std::cout
  << "N. of queries: " << queries.size() << std::endl
  << "Avg. returned: " << returned/queries.size() << std::endl
  << "Avg. mathcing: " << matching/queries.size() << std::endl
  << "Query time (us): " << q_time/queries.size() << std::endl
  << "Verification time (us): " << v_time/queries.size() << std::endl;
  // Before closing, free the memory occupied by the index.
  delete_tree(root);
  return 0;
}
