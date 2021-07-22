/**
 *  @file TestIndex.cpp
 *  @author Matteo Loporchio
 *
 *  This program can be used to test the construction
 *  of a MR-tree index from a set of records.
 *  In order to run it, the following parameters are required:
 *
 *    TestIndex <filename> <capacity> [<rect_file>]
 */

#include "Node.hpp"
#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std::chrono;

/**
 *  Given a node, saves the bounding rectangles of its children to a file.
 *  @param n the tree node
 */
void save_rect(Node *n, const std::string &filename) {
  if (!n) return;
  std::ofstream out;
  out.open(filename);
  out << "lx,ly,ux,uy\n";
  std::vector<Node*> children = ((IntNode *) n)-> getChildren();
  for (Node *child : children) {
    Rectangle r = child->getRect();
    // Write to file.
    out << r.lx << ',' << r.ly << ',' << r.ux << ',' << r.uy << '\n';
  }
  out.close();
}

int main(int argc, char **argv) {
  // Check the number of input arguments.
  if (argc < 3) {
    std::cerr << "Usage: " << argv[0] <<
    " <filename> <capacity> [<rect_file>]" << std::endl;
    return 1;
  }
  // Read the input parameters.
  std::string filename = argv[1];
  size_t capacity = atol(argv[2]);
  // Check if the optional parameter (i.e. filename for logging rectangles)
  // has been specified. If this is the case, initialize it.
  char *rect_file = ((argc == 4) ? argv[3] : NULL);
  // Open the input file and parse its content.
  std::vector<Record> records = load_file(filename);
  // Build the MR-tree index.
  auto start = high_resolution_clock::now();
  Node *root = packed(records, capacity);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  // If necessary, save the MBRs in the root to a file.
  if (rect_file) save_rect(root, std::string(rect_file));
  // Print the results.
  std::cout
  << "Construction time (us): " << duration.count() << std::endl
  << "Leaves: " << count_leaves(root) << std::endl
  << "Height: " << height(root) << std::endl;
  // Free the memory occupied by the index.
  delete_tree(root);
  return 0;
}
