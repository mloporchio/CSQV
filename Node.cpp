/**
 *  @file Node.cpp
 *  @author Matteo Loporchio
 */

#include "Node.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <limits>
#include <queue>
#include <cmath>

/**
 *  Creates a new leaf node from a list of records.
 *  @param data the list of records
 *  @return a leaf MR-tree node
 */
LeafNode *make_leaf(std::vector<Record> &data) {
  Rectangle rect = empty();
  Buffer buf;
  for (Record &e : data) {
    rect = enlarge(rect, e.loc);
    put_record(buf, e);
  }
  return new LeafNode(rect, sha256(buf), data);
}

/**
 *  Creates a new internal node from a list of child nodes.
 *  The new node is the parent of all these children.
 *  @param data the list of child nodes
 *  @return an internal MR-tree node
 */
IntNode *make_internal(std::vector<Node*> &children) {
  Rectangle rect = empty();
  Buffer buf;
  for (Node *child : children) {
    if (!child) continue;
    Rectangle r = child -> getRect();
    hash_t h = child -> getHash();
    rect = enlarge(rect, r);
    buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy).put_bytes(h.data(), h.size());
  }
  return new IntNode(rect, sha256(buf), children);
}

/**
 *  Builds a MR-tree from a list of records using a bulk-loading
 *  (a.k.a packed) algorithm.
 *  @param data list of records
 *  @param c page capacity
 *  @return a pointer to the root node of the tree
 */
Node *packed(std::vector<Record> &data, size_t c) {
  size_t l = 0, r = 0;
  // Pre-compute the number of leaves and initialize the vector.
  std::vector<Node*> curr;
  curr.reserve((data.size() / c) + ((data.size() % c) != 0));
  // Sort the points in ascending order.
  std::sort(data.begin(), data.end());
  // Split the records into chunks of size c
  // and then create a leaf node for each chunk.
  for (l = 0; l < data.size(); l += c) {
    r = std::min(data.size(), l+c);
    std::vector<Record> temp;
    temp.reserve(r-l);
    std::copy(data.begin()+l, data.begin()+r, std::back_inserter(temp));
    curr.push_back(make_leaf(temp));
  }
  // Start merging.
  while (curr.size() > 1) {
    // Divide the list of current nodes into chunks.
    // Then, for each chunk, merge all its nodes and create a new one.
    std::vector<Node*> merged;
    merged.reserve((curr.size() / c) + ((curr.size() % c) != 0));
    for (l = 0; l < curr.size(); l += c) {
      r = std::min(curr.size(), l+c);
      std::vector<Node*> temp;
      temp.reserve(r-l);
      std::copy(curr.begin()+l, curr.begin()+r, std::back_inserter(temp));
      merged.push_back(make_internal(temp));
    }
    // These nodes become the new working set.
    curr = std::move(merged);
  }
  // Return the root of the MR-tree, i.e. the only node left.
  return curr.at(0);
}

/**
 *  This function can be used to free the memory occupied by
 *  an entire MR-tree.
 *  @param r pointer to the tree root
 */
void delete_tree(Node *r) {
  // Check if the input is legal.
  if (!r) return;
  // Create an empty queue and insert the root node.
  std::queue<Node*> q;
  q.push(r);
  Node *curr = NULL;
  while (!q.empty()) {
    // Delete each node in the queue after pushing their children.
    curr = q.front();
    q.pop();
    // If we have reached an internal node,
    // we first enqueue its children for exploration.
    if (curr->getType() == N_INT) {
      std::vector<Node*> children = ((IntNode*) curr)->getChildren();
      for (Node *child : children) q.push(child);
    }
    // At this point, we can delete the current node.
    delete curr;
  }
  // Set the root as null before returning.
  r = NULL;
}

/**
 *  Counts the number of leaf nodes in the tree.
 *  @param r pointer to the tree root
 *  @return the number of leaves
 */
size_t count_leaves(Node *r) {
  // Check if the input is legal.
  if (!r) return 0;
  size_t count = 0;
  // Create an empty queue and insert the root node.
  std::queue<Node*> q;
  q.push(r);
  Node *curr = NULL;
  while (!q.empty()) {
    curr = q.front();
    q.pop();
    // If we have reached a leaf, increase the counter.
    if (curr->getType() == N_LEAF) count++;
    // Otherwise, enqueue the children of the internal node.
    else {
      std::vector<Node*> children = ((IntNode*) curr)->getChildren();
      for (Node *child : children) q.push(child);
    }
  }
  return count;
}

/**
 *  Computes the height of the tree.
 *  @param r pointer to the tree root
 *  @param the height of the tree
 */
size_t height(Node *r) {
  // Check if the input is legal.
  if (!r) return 0;
  if (r->getType() == N_LEAF) return 0;
  // If the node is internal, call the function recursively.
  size_t hmax = -std::numeric_limits<size_t>::infinity();
  std::vector<Node*> children = ((IntNode*) r)->getChildren();
  for (Node *child : children) hmax = std::max(hmax, height(child));
  return 1 + hmax;
}
