/**
 *  @file main.cpp
 *  @author Matteo Loporchio
 */

#include "Hash.hpp"
#include "Node.hpp"
#include "Query.hpp"
#include "Utils.hpp"
#include <chrono>
#include <iostream>
#include <random>

using namespace std::chrono;

int main(int argc, char **argv) {
  std::string filename = "Test/Data/2020_DATA_SA_Crash.csv";
  std::vector<Record> records = readFile(filename);
  Node *root = packed(records, 50);
  if (!root) {
    std::cerr <<
    "Error: something went wrong while building the index." << std::endl;
    return 1;
  }
  Rectangle r = root -> getRect();
  //std::cout << "MBR: (" << r.lx << ", " << r.ly << ", " << r.ux << ", " << r.uy << ")" << std::endl;
  std::cout << "Hash: " << toHex(root -> getHash()) << std::endl;
  // Time the execution of the query algorithm.
  auto q_start = high_resolution_clock::now();
  VObject *vo = query_it(root, {600000, 1400000, 1400000, 1400000});
  auto q_stop = high_resolution_clock::now();
  auto q_duration = duration_cast<microseconds>(q_stop - q_start);
  // Time the execution of the verification algorithm.
  auto v_start = high_resolution_clock::now();
  VResult *vr = verify(vo);
  auto v_stop = high_resolution_clock::now();
  auto v_duration = duration_cast<microseconds>(v_stop - v_start);
  Rectangle s = vr -> getRect();
  //std::cout << "MBR: (" << s.lx << ", " << s.ly << ", " << s.ux << ", " << s.uy << ")" << std::endl;
  std::cout << "Reconstructed hash: " << toHex(vr -> getHash()) << std::endl;
  std::cout << "Returned records: " << (vr->getData()).size() << std::endl;
  std::cout << "Query time (us): " << q_duration.count() << std::endl
  << "Verification time (us): " << v_duration.count() << std::endl;
  delete_tree(root);
  return 0;
}

//Rectangle r = root -> rect;
//std::cout << "MBR: (" << r.lx << ", " << r.ly << ", " << r.ux << ", " << r.uy << ")" << std::endl;
//std::cout << "Hash: " << toHex(root -> hash) << std::endl;
/*
size_t n = 1000;
double xmin = 0, ymin = 0, xmax = 100, ymax = 100;
std::uniform_real_distribution<double> xdist(xmin, xmax);
std::uniform_real_distribution<double> ydist(ymin, ymax);
std::default_random_engine re;
std::vector<Point> pts;
for (size_t i = 0; i < n; i++) {
  Point p = new Point{xdist(re), ydist(re)};
  pts.push_back(p);
}
Node *root = buildPacked(&pts, 50);
*/
