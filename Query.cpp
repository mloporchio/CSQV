/**
 *  @file Query.cpp
 *  @author Matteo Loporchio
 */

#include "Query.hpp"
#include <deque>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <memory>
#include <utility>
#include <iostream>
#include <cassert>

/**
 *  This function can be used to query the MR-tree index in order
 *  to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the tree root.
 *  @param r the root of the MR-tree
 *  @param q the query rectangle
 *  @return a VO for the root
 */
VObject *query(Node *r, const Rectangle &q) {
  // Check if the input is legal.
  if (!r) return NULL;
  // If the node is a leaf, we construct a VO with all its points.
  if (r->getType() == N_LEAF) {
    return new VLeaf(((LeafNode*) r)->getData());
  }
  // Otherwise, we need to check if the MBR of the node intersects
  // the query rectangle.
  // If this is not the case, then the subtree will not contain any
  // interesting record.
  Rectangle rect = r->getRect();
  if (!intersect(rect, q)) {
    return new VPruned(rect, r->getHash());
  }
  // Otherwise, we need to explore recursively all subtrees rooted in
  // the current node.
  VContainer *cont = new VContainer();
  std::vector<Node*> children = ((IntNode*) r)->getChildren();
  for (Node *c : children) {
    VObject *partial = query(c, q);
    cont->append(partial);
  }
  return cont;
}

/**
 *  This function can be used to query the MR-tree index iteratively
 *  in order to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the tree root.
 *  @param r the root of the MR-tree
 *  @param q the query rectangle
 *  @return a VO for the root
 */
VObject *query_it(Node *r, const Rectangle &q) {
  // Check if the input is legal.
  if (!r) return NULL;
  std::deque<std::pair<Node*,VObject*>> queue;
  queue.push_back(std::make_pair(r, ((VObject*) NULL)));
  VObject *result = NULL;
  while (!queue.empty()) {
    // Dequeue the first element.
    std::pair<Node*,VObject*> curr = queue.front();
    queue.pop_front();
    Node *currNode = curr.first;
    VObject *parentVO = curr.second, *currVO = NULL;
    // If the current node is a leaf, we construct a VO with all its points.
    if (currNode->getType() == N_LEAF) {
      std::vector<Record> data = ((LeafNode*) currNode)->getData();
      currVO = new VLeaf(data);
    }
    // Otherwise, we have reached an internal node.
    else {
      // If its bounding rectangle does not intersect the query, it is pruned.
      Rectangle currRect = currNode->getRect();
      if (!intersect(currRect, q)) {
        currVO = new VPruned(currRect, currNode->getHash());
      }
      // Otherwise, the corresponding subtree is queued for exploration.
      else {
        currVO = new VContainer();
        std::vector<Node*> children = ((IntNode*)currNode) -> getChildren();
        for (Node *n : children) queue.push_back(std::make_pair(n, currVO));
      }
    }
    // If the current node has a parent,
    if (parentVO) ((VContainer*) parentVO)->append(currVO);
    else result = currVO;
  }
  return result;
}

/**
 *  This method can be used to recursively reconstruct
 *  the root of the MR-tree index from a given verification object.
 *  The output of this method is a
 *  <code>VResult</code> object that contains the reconstructed result set
 *  together with the bounding rectangle and digest of the root node.
 *  @param vo a verification object
 *  @return the reconstructed information
 */
VResult *verify(VObject *vo) {
  // Check if the input is legal.
  if (!vo) return NULL;
  // Check the type of the VO.
  VObjectType type = vo->getType();
  // If the VO corresponds to a leaf, we reconstruct the node
  // by computing its MBR and hash from the set of record it contains.
  if (type == V_LEAF) {
    std::vector<Record> data = ((VLeaf *) vo)->getData();
    Rectangle rect = empty();
    Buffer buf;
    for (Record &e : data) {
      rect = enlarge(rect, e.loc);
      hash_t digest = hash_record(e);
      buf.put_bytes(digest.data(), digest.size());
    }
    return new VResult(rect, sha256(buf), data);
  }
  // If the VO corresponds to a pruned internal node, we reconstruct
  // using the provided MBR and digest.
  if (type == V_PRUNED) {
    VPruned *pr = (VPruned *) vo;
    std::vector<Record> data;
    return new VResult(pr->getRect(), pr->getHash(), data);
  }
  // Otherwise we must reconstruct a non-pruned internal node.
  // This node is represented by a VO container.
  VContainer *cont = (VContainer *) vo;
  std::vector<Record> data;
  Rectangle rect = empty();
  Buffer buf;
  // Recursively examine each VO in the container.
  for (size_t i = 0; i < cont->size(); i++) {
    VResult *partial = verify((VObject*) cont->get(i));
    // Take all the matching records and add them to the result set.
    std::vector<Record> vdata = partial->getData();
    data.insert(std::end(data), std::begin(vdata), std::end(vdata));
    // Compute the hash and bounding rectangle.
    Rectangle r = partial->getRect();
    hash_t h = partial->getHash();
    rect = enlarge(rect, r);
    buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy).put_bytes(h.data(), h.size());
  }
  return new VResult(rect, sha256(buf), data);
}

/**
 *  This method can be used to iteratively reconstruct
 *  the root of the MR-tree index from a given verification object.
 *  The output of this method is a <code>VResult</code> object
 *  that contains the reconstructed result set
 *  together with the bounding rectangle and digest of the root node.
 *  @param vo a verification object
 *  @return the reconstructed information
 */
VResult *verify_it(VObject *vo) {
  // Check if the input is legal.
  if (!vo) return NULL;
  VResult *result = NULL;
  std::stack<std::pair<VObject*, VObject*>> stack;
  std::map<VObject*, bool> visited;
  std::map<VObject*, std::vector<VResult*>> content;
  stack.push(std::make_pair(vo, (VObject*) NULL));
  visited[vo] = false;
  while (!stack.empty()) {
    std::pair<VObject*,VObject*> el = stack.top();
    VObject *curr = el.first, *parent = el.second;
    // Check the type of the current VO.
    VObjectType type = curr->getType();
    // If it is a container...
    if (type == V_CONTAINER) {
      // Check if the corresponding node has already been visited.
      // If this is not the case, we push its children on the stack
      // (in reverse order) for exploration.
      if (!visited[vo]) {
        VContainer *container = ((VContainer*) curr);
        content[curr] = std::vector<VResult*>();
        for (int i = ((int) container->size())-1; i >= 0; i--) {
          VObject *child = container->get(i);
          stack.push(std::make_pair(child, curr));
          visited[child] = false;
        }
        visited[curr] = true;
      }
      // On the other hand, if the node has already been visited
      // we have to reconstruct.
      else {
        //std::vector<VResult*> curr_content = curr->getContent();//content[curr];
        std::vector<Record> data;
        Rectangle rect = empty();
        Buffer buf;
        for (VResult *vr : content[curr]) {
          // Collect all records.
          std::vector<Record> vdata = vr->getData();
          data.insert(std::end(data), std::begin(vdata), std::end(vdata));
          // Compute the MBR and hashes.
          Rectangle r = vr->getRect();
          hash_t h = vr->getHash();
          rect = enlarge(rect, r);
          buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy)
          .put_bytes(h.data(), h.size());
        }
        VResult *partial = new VResult(rect, sha256(buf), data);
        if (parent) content[parent].push_back(partial);
        else result = partial;
        stack.pop();
      }
    }
    // Otherwise we have reached a leaf or pruned node.
    else {
      VResult *partial = NULL;
      // If the node is a leaf, reconstruct its MBR and hash from
      // the records it contains.
      if (type == V_LEAF) {
        std::vector<Record> data = ((VLeaf*) curr)->getData();
        Rectangle rect = empty();
        Buffer buf;
        for (Record &e : data) {
          rect = enlarge(rect, e.loc);
          hash_t digest = hash_record(e);
          buf.put_bytes(digest.data(), digest.size());
        }
        partial = new VResult(rect, sha256(buf), data);
      }
      // Otherwise, if the node is pruned, we reconstruct using
      // its MBR and hash.
      else {
        VPruned *pr = (VPruned*) curr;
        std::vector<Record> data;
        partial = new VResult(pr->getRect(), pr->getHash(), data);
      }
      if (parent) content[parent].push_back(partial);
      else result = partial;
      // In both cases, we pop the node from the stack.
      stack.pop();
    }
  }
  return result;
}
