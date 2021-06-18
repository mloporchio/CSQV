/**
 *  @file Node.cpp
 *  @author Matteo Loporchio
 */

#include "Node.hpp"
#include "Utils.hpp"
#include <queue>
#include <sstream>
#include <iostream>

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
    hash_t digest = hash_record(e);
    buf.put_bytes(digest.data(), digest.size());
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
    if (child) {
      Rectangle r = child -> getRect();
      hash_t h = child -> getHash();
      rect = enlarge(rect, r);
      buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy)
      .put_bytes(h.data(), h.size());
    }
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
  std::vector<Node*> current;
  // Sort the points in ascending order.
  std::sort(data.begin(), data.end());
  // Split the records into chunks of size c.
  // Then create a leaf node for each chunk.
  std::vector<std::vector<Record>> leaves = split(data, c);
  for (size_t i = 0; i < leaves.size(); i++) {
    Node *l = make_leaf(leaves[i]);
    current.push_back(l);
  }
  // Start merging.
  while (current.size() > 1) {
    // Divide the list of current nodes into chunks.
    std::vector<std::vector<Node*>> chunks = split(current, c);
    // For each chunk, merge all its nodes and create a new one.
    std::vector<Node*> merged;
    for (size_t i = 0; i < chunks.size(); i++) {
      Node *n = make_internal(chunks[i]);
      merged.push_back(n);
    }
    // These nodes become the new working set.
    current = merged;
  }
  // Return the root of the MR-tree, i.e. the only node left.
  return current.at(0);
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
  Node *curr = nullptr;
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
  r = nullptr;
}
