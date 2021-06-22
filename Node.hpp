/**
 *  @file Node.hpp
 *  @author Matteo Loporchio
 */

#ifndef NODE_H
#define NODE_H

#include "Geometry.hpp"
#include "Hash.hpp"
#include "Record.hpp"

/**
 *  This enum defines the kind of a MR-tree node, which can be
 *  either a leaf or an internal node.
 */
enum NodeType {N_LEAF, N_INT};

/**
 *  This class represents a generic node of the MR-tree.
 *  For each node we keep track of its type (either leaf or internal),
 *  its MBR (i.e. the MBR of all records in the subtree) and its digest.
 */
class Node {
private:
  NodeType type; ///< Node type indicator
  Rectangle rect; ///< Bounding rectangle
  hash_t hash; ///< The digest of the node
public:
  /**
   *  Default class constructor.
   *  @param t the node type
   *  @param r the MBR of the node
   *  @param h the hash value of the node
   */
  Node(NodeType t, Rectangle r, hash_t h) : type(t), rect(r), hash(h) {}

  /**
   *  Default destructor.
   */
  virtual ~Node() {}

  /**
   *  Returns the type of the node.
   *  @return the node type
   */
  NodeType getType() {return type;}

  /**
   *  Returns the MBR of the node.
   *  @return the bounding rectangle of the node
   */
  Rectangle getRect() {return rect;}

  /**
   *  Returns the digest of the node.
   *  @return the node digest
   */
  hash_t getHash() {return hash;}
};

/**
 *  This class represents a leaf node.
 */
class LeafNode : public Node {
private:
  std::vector<Record> data; ///< A leaf node stores a list of records.
public:
  /**
   *  Default class constructor.
   *  @param r the node rectangle
   *  @param h the hash value of the node
   *  @param data a list of records to be stored
   */
  LeafNode(Rectangle r, hash_t h, std::vector<Record> data)
  : Node(N_LEAF, r, h), data(data) {}

  /**
   *  Returns the list of records contained in the node.
   *  @return the list of records
   */
  std::vector<Record> getData() {return data;}
};

/**
 *  This class represents an internal node of the tree.
 */
class IntNode : public Node {
private:
  std::vector<Node*> children; ///< A list of pointers to the children
public:
  /**
   *  @param r the node rectangle
   *  @param h the hash value of the node
   *  @return
   */
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
