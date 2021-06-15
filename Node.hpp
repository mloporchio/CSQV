/**
 *  @file Node.hpp
 *  @author Matteo Loporchio
 */

#ifndef NODE_H
#define NODE_H

#include "Geometry.hpp"
#include "Hash.hpp"
#include "Record.hpp"

enum NodeType {N_LEAF, N_INT};

/**
 *
 */
class Node {
private:
  NodeType type;
  Rectangle rect;
  hash_t hash;
public:
  Node(NodeType t, Rectangle r, hash_t h) : type(t), rect(r), hash(h) {}
  virtual ~Node() {}
  NodeType getType() {return type;}
  Rectangle getRect() {return rect;}
  hash_t getHash() {return hash;}
};

/**
 *
 */
class LeafNode : public Node {
private:
  std::vector<Record> data;
public:
  LeafNode(Rectangle r, hash_t h, std::vector<Record> data)
  : Node(N_LEAF, r, h), data(data) {}
  std::vector<Record> getData() {return data;}
};

/**
 *
 */
class IntNode : public Node {
private:
  std::vector<Node*> children;
public:
  IntNode(Rectangle r, hash_t h, std::vector<Node*> children)
  : Node(N_INT, r, h), children(children) {}
  std::vector<Node*> getChildren() {return children;}
};

/**
 *  Creates a new leaf node from a list of records.
 *  @param data the list of records
 *  @return a leaf MR-tree node
 */
LeafNode *make_leaf(std::vector<Record> &data);

/**
 *  Creates a new internal node from a list of child nodes.
 *  The new node is the parent of all these children.
 *  @param data the list of child nodes
 *  @return an internal MR-tree node
 */
IntNode *make_internal(std::vector<Node*> &children);

/**
 *  Builds a MR-tree from a list of records using a bulk-loading
 *  (a.k.a packed) algorithm.
 *  @param data list of records
 *  @param c page capacity
 *  @return a pointer to the root node of the tree
 */
Node *packed(std::vector<Record> &data, size_t c);

/**
 *  This function can be used to free the memory occupied by
 *  an entire MR-tree.
 *  @param r pointer to the tree root
 */
void delete_tree(Node *r);

#endif
