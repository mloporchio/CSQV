/**
 *  @file Query.cpp
 *  @author Matteo Loporchio
 */

#include "Query.hpp"
#include <deque>
#include <map>
#include <stack>
#include <utility>
#include <iostream>

/**
 *  Counts the number of records inside the verification object.
 *  @param vo a verification object
 *  @return the number of records inside the object
 */
size_t count_records(VObject *vo) {
  // Check if the input is legal.
  if (!vo) return 0;
  // Check the type of the VO.
  VObjectType type = vo->getType();
  // If the VO corresponds to a leaf, we return the number of its elements.
  if (type == V_LEAF) return (((VLeaf *) vo)->getData()).size();
  // If the VO corresponds to a pruned internal node, the result is zero.
  if (type == V_PRUNED) return 0;
  // Otherwise the VO has been generated for a non-pruned internal node.
  // We recursively examine each VO in the container.
  VContainer *cont = (VContainer *) vo;
  size_t count = 0;
  for (size_t i = 0; i < cont->size(); i++)
    count += count_records((VObject*) cont->get(i));
  return count;
}

/**
 *  This function can be used to query the MR-tree index recursively
 *  to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the root node.
 *  @param r the root of the MR-tree
 *  @param q the query rectangle
 *  @return a VO for the root
 */
VObject *query(Node *r, const Rectangle &q) {
  // Check if the input is legal.
  if (!r) return NULL;
  // If the node is a leaf, we construct a VO with all its points.
  if (r->getType() == N_LEAF) {
    return new VLeaf(std::move(((LeafNode*) r)->getData()));
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
  const std::vector<Node*> &children = ((IntNode*) r)->getChildren();
  for (Node *c : children) {
    VObject *partial = query(c, q);
    cont->append(partial);
  }
  return cont;
}

/**
 *  This method can be used to reconstruct the MR-tree index root recursively.
 *  The output of this method is a <code>VResult</code> object
 *  that contains the result set, the bounding rectangle and digest of the root.
 *  @param vo a verification object
 *  @param q the query rectangle
 *  @return the reconstructed information
 */
VResult *verify(VObject *vo, const Rectangle &q) {
  // Check if the input is legal.
  if (!vo) return NULL;
  // Check the type of the VO.
  VObjectType type = vo->getType();
  // If the VO corresponds to a leaf, we reconstruct the node
  // by computing its MBR and hash from the set of record it contains.
  if (type == V_LEAF) {
    std::vector<Record> v_data = ((VLeaf *) vo)->getData(), data;
    data.reserve(v_data.size());
    Buffer buf(v_data.size() * sizeof(Record));
    Rectangle rect = empty();
    for (Record &e : v_data) {
      rect = enlarge(rect, e.loc);
      put_record(buf, e);
      if (match(e, q)) data.push_back(e);
    }
    return new VResult(rect, sha256(buf), std::move(data));
  }
  // If the VO corresponds to a pruned internal node, we reconstruct
  // using the provided MBR and digest.
  if (type == V_PRUNED) {
    VPruned *pr = (VPruned *) vo;
    return new VResult(pr->getRect(), pr->getHash(), std::vector<Record>());
  }
  // Otherwise we must reconstruct a non-pruned internal node.
  // This node is represented by a VO container.
  VContainer *cont = (VContainer *) vo;
  std::vector<Record> data;
  Buffer buf(cont->size() * ENTRY_SIZE);
  Rectangle rect = empty();
  // Recursively examine each VO in the container.
  for (size_t i = 0; i < cont->size(); i++) {
    VResult *partial = verify((VObject*) cont->get(i), q);
    // Take all the matching records and add them to the result set.
    std::vector<Record> v_data = partial->getData();
    std::move(v_data.begin(), v_data.end(), std::back_inserter(data));
    // Compute the hash and bounding rectangle.
    Rectangle r = partial->getRect();
    hash_t h = partial->getHash();
    rect = enlarge(rect, r);
    buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy).put_bytes(h.data(), h.size());
  }
  return new VResult(rect, sha256(buf), std::move(data));
}

/**
 *  This function can be used to query the MR-tree index iteratively
 *  to retrieve all points that belong to the query rectangle.
 *  The function returns a verification object for the root node.
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
      currVO = new VLeaf(std::move(((LeafNode*) currNode)->getData()));
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
 *  This method can be used to reconstruct the MR-tree index root iteratively.
 *  The output of this method is a <code>VResult</code> object
 *  that contains the result set, the bounding rectangle and digest of the root.
 *  @param vo a verification object
 *  @param q the query rectangle
 *  @return the reconstructed information
 */
VResult *verify_it(VObject *vo, const Rectangle &q) {
  VResult *result = NULL;
  // First, we check if the input is legal.
  if (!vo) return result;
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
      if (!visited[curr]) {
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
        std::vector<VResult*> partial_results = content[curr];
        std::vector<Record> data;
        Buffer buf(partial_results.size() * ENTRY_SIZE);
        Rectangle rect = empty();
        for (VResult *vr : partial_results) {
          // Collect all records.
          std::vector<Record> v_data = vr->getData();
          std::move(v_data.begin(), v_data.end(), std::back_inserter(data));
          // Compute the MBR and hashes.
          Rectangle r = vr->getRect();
          hash_t h = vr->getHash();
          rect = enlarge(rect, r);
          buf.put(r.lx).put(r.ly).put(r.ux).put(r.uy)
          .put_bytes(h.data(), h.size());
        }
        VResult *partial = new VResult(rect, sha256(buf), std::move(data));
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
        std::vector<Record> v_data = ((VLeaf *) curr)->getData(), data;
        data.reserve(v_data.size());
        Buffer buf(v_data.size() * sizeof(Record));
        Rectangle rect = empty();
        for (Record &e : v_data) {
          rect = enlarge(rect, e.loc);
          put_record(buf, e);
          if (match(e, q)) data.push_back(e);
        }
        partial = new VResult(rect, sha256(buf), std::move(data));
      }
      // Otherwise, if the node is pruned, we reconstruct using
      // its MBR and hash.
      else {
        VPruned *pr = (VPruned*) curr;
        partial = new VResult(pr->getRect(), pr->getHash(),
        std::vector<Record>());
      }
      if (parent) content[parent].push_back(partial);
      else result = partial;
      // In both cases, we pop the node from the stack.
      stack.pop();
    }
  }
  return result;
}
