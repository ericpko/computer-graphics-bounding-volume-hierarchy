#include "find_all_intersecting_pairs_using_AABBTrees.h"
#include "box_box_intersect.h"
// Hint: use a list as a queue
#include <list>

void find_all_intersecting_pairs_using_AABBTrees(
  const std::shared_ptr<AABBTree> & rootA,
  const std::shared_ptr<AABBTree> & rootB,
  std::vector<std::pair<std::shared_ptr<Object>,std::shared_ptr<Object> > > &
    leaf_pairs)
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * The items go into the front of the list and out the back.
   */
  // Use a list as a queue
  std::list<std::pair<std::shared_ptr<AABBTree>, std::shared_ptr<AABBTree>>> Q;

  // Add the roots to the list
  Q.emplace_front(rootA, rootB);

  while (!Q.empty()) {
    // Pop the first pair in the list
    std::shared_ptr<AABBTree> nodeA = Q.back().first;
    std::shared_ptr<AABBTree> nodeB = Q.back().second;
    Q.pop_back();

    if (!box_box_intersect(nodeA->box, nodeB->box)) {
      // If they don't intersect, then don't worry about it
      continue;
    }

    // They definitely intersect past here --- else ----
    // Now we have to check for leaves if we can add to leaf_pairs


    // Case 1: Both nodes have leaves
    if (nodeA->num_leaves <= 2 && nodeB->num_leaves <= 2) {
      if (nodeA->left && nodeB->left) { // check that they exist (not NULL)
        leaf_pairs.emplace_back(nodeA->left, nodeB->left);
      }
      if (nodeA->left && nodeB->right) {
        leaf_pairs.emplace_back(nodeA->left, nodeB->right);
      }
      if (nodeA->right && nodeB->left) {
        leaf_pairs.emplace_back(nodeA->right, nodeB->left);
      }
      if (nodeA->right && nodeB->right) {
        leaf_pairs.emplace_back(nodeA->right, nodeB->right);
      }


    // Case 2: Only nodeA has leaves or nodeB has leaves, but not both
    } else if (nodeA->num_leaves <= 2) {
      Q.emplace_front(nodeA, std::static_pointer_cast<AABBTree>(nodeB->left));
      Q.emplace_front(nodeA, std::static_pointer_cast<AABBTree>(nodeB->right));

    } else if (nodeB->num_leaves <= 2) {
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->left), nodeB);
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->right), nodeB);


    // Case 3: Neither node has leaves, so queue both children nodes
    } else {
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->left), std::static_pointer_cast<AABBTree>(nodeB->left));
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->left), std::static_pointer_cast<AABBTree>(nodeB->right));
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->right), std::static_pointer_cast<AABBTree>(nodeB->left));
      Q.emplace_front(std::static_pointer_cast<AABBTree>(nodeA->right), std::static_pointer_cast<AABBTree>(nodeB->right));
    }
  }
  ////////////////////////////////////////////////////////////////////////////
}
