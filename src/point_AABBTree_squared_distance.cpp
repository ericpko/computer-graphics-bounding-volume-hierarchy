#include "point_AABBTree_squared_distance.h"
#include <queue> // std::priority_queue
#include <limits>

typedef std::pair<double, std::shared_ptr<AABBTree> > MyPair;

bool point_AABBTree_squared_distance(
    const Eigen::RowVector3d & query,
    const std::shared_ptr<AABBTree> & root,
    const double min_sqrd,
    const double max_sqrd,
    double & sqrd,
    std::shared_ptr<Object> & descendant)
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * They key of our priority queue is represented as a std::pair of
   * double, and ptr to AABBTree, and the key is the double of the pair.
   * i.e. the squared distance from query to root bbox.
   * We are following the algorithm from class on
   * Nearest Neighbour / Closest Point. See ipad Oct 2 notes.
   */

  // Set up some variables
  bool found = false;
  double min_sqrd_dist = std::numeric_limits<double>::infinity();

  // Priority queue of a pair, and a vector of pairs
  // NOTE: std::greater will compare by the first element of the pair
  std::priority_queue<MyPair, std::vector<MyPair>, std::greater<MyPair> > PQ;

  // Add the first pair<double, ptr->tree> to the queue
  // NOTE: We use emplace, otherwise we need PQ.push(make_pair(...))
  PQ.emplace(point_box_squared_distance(query, root->box), root);

  while (!PQ.empty()) {
    // Remove the top node from the PQ
    double sqrd_dist = PQ.top().first;
    std::shared_ptr<AABBTree> aabb_ptr = PQ.top().second;
    PQ.pop();

    // Check if we have leaves (primitive objects) below
    if (aabb_ptr->num_leaves <= 2) {
      // Then we can get the squared distance to each object in the leaves

      // Then we have two primitives (objects)
      double _sqrd;
      std::shared_ptr<Object> _descendant;

      // Get the squared distance from query to each primitive
      if (aabb_ptr->left) {
        if (aabb_ptr->left->point_squared_distance(query, min_sqrd, max_sqrd, _sqrd, _descendant)) {
          if (_sqrd < min_sqrd_dist) {
            min_sqrd_dist = _sqrd;
            sqrd = min_sqrd_dist;
            descendant = aabb_ptr->left;
            found = true;
          }
        }
      }
      if (aabb_ptr->right) {
        if (aabb_ptr->right->point_squared_distance(query, min_sqrd, max_sqrd, _sqrd, _descendant)) {
          if (_sqrd < min_sqrd_dist) {
            min_sqrd_dist = _sqrd;
            sqrd = min_sqrd_dist;
            descendant = aabb_ptr->right;
            found = true;
          }
        }
      }

    } else if (sqrd_dist > min_sqrd_dist) {
      // Then we're not at a leaf, so check if the squred distance from query
      // to the bounding box is greater than the min so far.
      ;     // do nothing

    } else {
      // Then we are in the middle of the tree, so we can enqueue the left
      // and right subtrees.
      MyPair left_pair = std::make_pair(point_box_squared_distance(query, aabb_ptr->left->box),
                                        std::static_pointer_cast<AABBTree>(aabb_ptr->left));
      MyPair right_pair = std::make_pair(point_box_squared_distance(query, aabb_ptr->right->box),
                                        std::static_pointer_cast<AABBTree>(aabb_ptr->right));
      PQ.emplace(left_pair);
      PQ.emplace(right_pair);
    }
  }

  return found;
  ////////////////////////////////////////////////////////////////////////////
}












/**
 * Version 2: Same as in-class algorithm, but modified a bit.
 */

// #include "point_AABBTree_squared_distance.h"
// #include <queue> // std::priority_queue
// #include <limits>

// typedef std::pair<double, std::shared_ptr<AABBTree> > MyPair;

// bool point_AABBTree_squared_distance(
//     const Eigen::RowVector3d & query,
//     const std::shared_ptr<AABBTree> & root,
//     const double min_sqrd,
//     const double max_sqrd,
//     double & sqrd,
//     std::shared_ptr<Object> & descendant)
// {
//   ////////////////////////////////////////////////////////////////////////////
//   /**
//    * They key of our priority queue is represented as a std::pair of
//    * double, and ptr to AABBTree, and the key is the double of the pair.
//    * i.e. the squared distance from query to root bbox.
//    * We are following the algorithm from class on
//    * Nearest Neighbour / Closest Point. See ipad Oct 2 notes.
//    */

//   // Set up some variables
//   bool found = false;
//   double min_sqrd_dist = std::numeric_limits<double>::infinity();

//   // Priority queue of a pair, and a vector of pairs
//   // NOTE: std::greater will compare by the first element of the pair
//   std::priority_queue<MyPair, std::vector<MyPair>, std::greater<MyPair> > PQ;

//   // Add the first pair<double, ptr->tree> to the queue
//   // NOTE: We use emplace, otherwise we need PQ.push(make_pair(...))
//   PQ.emplace(point_box_squared_distance(query, root->box), root);

//   while (!PQ.empty()) {
//     // Remove the top node from the PQ
//     double sqrd_dist_to_bbox = PQ.top().first;
//     std::shared_ptr<AABBTree> aabb_ptr = PQ.top().second;
//     PQ.pop();

//     // Check the squared distance to the outtermost bounding box
//     if (sqrd_dist_to_bbox < min_sqrd_dist) {

//       // Then we need to check the distance to primative objects (leaves)
//       if (aabb_ptr->num_leaves < 3) {
//         // Then we can get the squared distance to each object in the leaves

//         // Then we have two primitives (objects)
//         double _sqrd;
//         std::shared_ptr<Object> _descendant;

//         // Get the squared distance from query to each primitive
//         if (aabb_ptr->left) {
//           if (aabb_ptr->left->point_squared_distance(query, min_sqrd, max_sqrd, _sqrd, _descendant)) {
//             if (_sqrd < min_sqrd_dist) {
//               min_sqrd_dist = _sqrd;
//               sqrd = min_sqrd_dist;
//               descendant = aabb_ptr->left;
//               found = true;
//             }
//           }
//         }
//         if (aabb_ptr->right) {
//           if (aabb_ptr->right->point_squared_distance(query, min_sqrd, max_sqrd, _sqrd, _descendant)) {
//             if (_sqrd < min_sqrd_dist) {
//               min_sqrd_dist = _sqrd;
//               sqrd = min_sqrd_dist;
//               descendant = aabb_ptr->right;
//               found = true;
//             }
//           }
//         }

//       } else {
//         // Then we are in the middle of the tree, so we can enqueue the left
//         // and right subtrees.
//         MyPair left_pair = std::make_pair(point_box_squared_distance(query, aabb_ptr->left->box),
//                                           std::static_pointer_cast<AABBTree>(aabb_ptr->left));
//         MyPair right_pair = std::make_pair(point_box_squared_distance(query, aabb_ptr->right->box),
//                                           std::static_pointer_cast<AABBTree>(aabb_ptr->right));
//         PQ.emplace(left_pair);
//         PQ.emplace(right_pair);
//       }

//     } else {
//       // If we get here, then the squared distance from query to the bound
//       // box is greater than the minimum sqrd distance found so far, so
//       // we don't want to add any subtrees to the queue since they can't
//       // possibly be less distance than their outermost bounding box
//       ;
//     }
//   }

//   return found;
//   ////////////////////////////////////////////////////////////////////////////
// }
