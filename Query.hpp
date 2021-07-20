/**
 *  @file Query.hpp
 *  @author Matteo Loporchio
 */

#ifndef QUERY_H
#define QUERY_H

#include "Node.hpp"

/**
 *  A verification object may refer to a leaf node,
 *  a pruned internal node or an explored subtree.
 */
enum VObjectType {V_LEAF, V_PRUNED, V_CONTAINER};

/**
 *  Base class for verification objects.
 */
class VObject {
protected:
  VObjectType type;
public:
  VObject(VObjectType t) : type(t) {}
  virtual ~VObject() {}
  VObjectType getType() {return type;}
};

/**
 *  Verification object for leaf nodes.
 */
class VLeaf : public VObject {
private:
  std::vector<Record> data;
public:
  VLeaf(std::vector<Record> data) : VObject(V_LEAF), data(std::move(data)) {}
  std::vector<Record> getData() {return data;}
};

/**
 *  Verification object for pruned internal node.
 */
class VPruned : public VObject {
private:
  Rectangle rect;
  hash_t hash;
public:
  VPruned(Rectangle r, hash_t h) : VObject(V_PRUNED), rect(r), hash(h) {}
  Rectangle getRect() {return rect;}
  hash_t getHash() {return hash;}
};

/**
 *  Verification object for explored internal node.
 */
class VContainer : public VObject {
private:
  std::vector<VObject*> list;
public:
  VContainer() : VObject(V_CONTAINER) {}
  void append(VObject *vo) {if (vo) list.push_back(vo);}
  VObject *get(size_t i) {return list.at(i);}
  size_t size() {return list.size();}
};

/**
 *  This class represents the output of the spatial verification algorithm.
 *  This object contains the result set of the query, the minimum bounding
 *  rectangle of the root of the MR-tree index and the digest of the node,
 *  all reconstructed from the verification object.
 */
class VResult {
private:
  Rectangle rect;
  hash_t hash;
  std::vector<Record> data;
public:
  VResult(Rectangle r, hash_t h, std::vector<Record> data) :
  rect(r), hash(h), data(std::move(data)) {};
  Rectangle getRect() {return rect;}
  hash_t getHash() {return hash;}
  std::vector<Record> getData() {return data;}
  size_t count() {return data.size();}
};

/**
 *  Counts the number of records inside the verification object.
 *  @param vo a verification object
 *  @return the number of records inside the object
 */
size_t count_records(VObject *vo);

/**
 *  This function can be used to query the MR-tree index recursively
 *  to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the root node.
 *  @param r the root of the MR-tree
 *  @param q the query rectangle
 *  @return a VO for the root
 */
VObject *query(Node *r, const Rectangle &q);

/**
 *  This method can be used to reconstruct the MR-tree index root recursively.
 *  The output of this method is a <code>VResult</code> object
 *  that contains the result set, the bounding rectangle and digest of the root.
 *  @param vo a verification object
 *  @param q the query rectangle
 *  @return the reconstructed information
 */
VResult *verify(VObject *vo, const Rectangle &q);

/**
 *  This function can be used to query the MR-tree index iteratively
 *  to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the root node.
 *  @param r the root of the MR-tree
 *  @param q the query rectangle
 *  @return a VO for the root
 */
VObject *query_it(Node *r, const Rectangle &q);

/**
 *  This method can be used to reconstruct the MR-tree index root iteratively.
 *  The output of this method is a <code>VResult</code> object
 *  that contains the result set, the bounding rectangle and digest of the root.
 *  @param vo a verification object
 *  @param q the query rectangle
 *  @return the reconstructed information
 */
VResult *verify_it(VObject *vo, const Rectangle &q);

#endif
