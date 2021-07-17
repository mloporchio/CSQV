/**
 *  @file QueryGen.cpp
 *  @author Matteo Loporchio
 *
 *  Given a data set, this program outputs a file with query rectangles,
 *  along with their pre-computed fraction of matching records.
 *  Queries are generated uniformly at random in a given 2D space.
 */

#include "Geometry.hpp"
#include "Record.hpp"
#include <fstream>
#include <iostream>
#include <random>

int main(int argc, char const *argv[]) {
  std::string data_file = argv[1];
  std::string query_file = argv[2];
  size_t n_queries = atol(argv[3]);
  // Open the input file and compute the MBR of the data set.
  std::vector<Record> records = load_file(data_file);
  Rectangle r = empty();
  for (Record &e : records) r = enlarge(r, e.loc);
  // Generate n_queries random rectangles inside the MBR.
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_int_distribution<int32_t> lxd(r.lx, r.ux), lyd(r.ly, r.uy);
  std::ofstream out;
  out.open(query_file);
  out << "lx,ly,ux,uy,matching,fraction\n";
  for (size_t i = 0; i < n_queries; i++) {
    // Generate a random query rectangle.
    int32_t lx = lxd(eng), ly = lyd(eng), ux, uy;
    std::uniform_int_distribution<int32_t> uxd(lx, r.ux), uyd(ly, r.uy);
    ux = uxd(eng); uy = uyd(eng);
    Rectangle rq = {lx, ly, ux, uy};
    // Compute the number of matching records and the fraction.
    size_t matching = count(records, rq);
    double fraction = ((double) matching) / ((double) records.size());
    // Write to file.
    out << lx << ',' << ly << ',' << ux << ',' << uy << ','
    << matching << ',' << fraction << '\n';
  }
  out.close();
  std::cout << "Done!" << std::endl;
  return 0;
}
