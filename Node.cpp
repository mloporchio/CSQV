/**
 *  @file Node.cpp
 *  @author Matteo Loporchio
 */

#include "Node.hpp"
#include <algorithm>
#include <limits>
#include <queue>

/**
 *  Creates a new leaf node from a list of records.
 *  @param data the list of records
 *  @return a leaf MR-tree node
 */
LeafNode *make_leaf(std::vector<Record> data) {
  Rectangle rect = empty();
  Buffer buf(data.size() * sizeof(Record));
  for (Record &e : data) {
    rect = enlarge(rect, e.loc);
    put_record(buf, e);
  }
  return new LeafNode(rect, sha256(buf), std::move(data));
}

/**
 *  Creates a new internal node from a list of child nodes.
 *  The new node is the parent of all these children.
 *  @param children the list of child nodes
 *  @return an internal MR-tree node
 */
IntNode *make_internal(std::vector<Node*> children) {
  Rectangle rect = empty();
  Buffer buf(children.size() * ENTRY_SIZE);
  for (Node *child : children) {
    Rectangle r = child -> getRect();
    hash_t h = child -> getHash();
    rect = enlarge(rect, r);
    buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy).put_bytes(h.data(), h.size());
  }
  return new IntNode(rect, sha256(buf), std::move(children));
}

/**
 *  Builds a MR-tree from a list of records using a bulk-loading algorithm.
 *  @param data list of records
 *  @param c page capacity
 *  @return a pointer to the root node of the tree
 */
Node *packed(std::vector<Record> &data, size_t c) {
  // Sort the records in ascending order.
  std::sort(data.begin(), data.end());
  // Create the leaves by splitting the list of points in chunks of size c.
  std::vector<Node*> curr;
  curr.reserve(N_PARTS(data.size(), c));
  for (size_t l = 0; l < data.size(); l += c) {
    size_t r = std::min(data.size(), l+c);
    std::vector<Record> temp(data.begin()+l, data.begin()+r);
    curr.push_back(make_leaf(std::move(temp)));
  }
  // Create the internal nodes. Split and merge until one node is left.
  while (curr.size() > 1) {
    std::vector<Node*> merged;
    merged.reserve(N_PARTS(curr.size(), c));
    for (size_t l = 0; l < curr.size(); l += c) {
      size_t r = std::min(curr.size(), l+c);
      std::vector<Node*> temp(curr.begin()+l, curr.begin()+r);
      merged.push_back(make_internal(std::move(temp)));
    }
    curr = merged;
  }
  // Return the only node left.
  return curr[0];
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
int count_leaves(Node *r) {
  // Check if the input is legal.
  if (!r) return -1;
  int count = 0;
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
int height(Node *r) {
  // Check if the input is legal.
  if (!r) return -1;
  if (r->getType() == N_LEAF) return 0;
  // If the node is internal, call the function recursively.
  int hmax = -1;
  std::vector<Node*> children = ((IntNode*) r)->getChildren();
  for (Node *child : children) hmax = std::max(hmax, height(child));
  return 1 + hmax;
}
