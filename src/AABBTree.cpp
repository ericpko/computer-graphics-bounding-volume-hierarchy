#include "AABBTree.h"
#include "insert_box_into_box.h"

AABBTree::AABBTree(
  const std::vector<std::shared_ptr<Object> > & objects,
  int a_depth):
  depth(std::move(a_depth)),
  num_leaves(objects.size())
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * We are going to follow the algorithm on pg 305
   */
  int N = num_leaves;                   // objects.size()
  if (N == 0) {                         // empty tree
    this->left = NULL;
    this->right = NULL;
  } else if (N == 1) {
    this->left = objects[0];
    this->right = NULL;
    insert_box_into_box(this->left->box, this->box);
  } else if (N == 2) {
    this->left = objects[0];
    this->right = objects[1];
    insert_box_into_box(this->left->box, this->box);
    insert_box_into_box(this->right->box, this->box);

  } else {
    /**
     * Put all the sub bounding boxes into this box.
     * NOTE: We have to do this before the bottom loop, otherwise we get
     * weird shaped boxes
     */
    for (std::shared_ptr<Object> obj_ptr : objects) {
      insert_box_into_box(obj_ptr->box, this->box);
    }

    // Find the longest longest_axis of the box
    double ax_len = std::fabs(this->box.max_corner[0] - this->box.min_corner[0]);
    int ax_longest = 0;
    for (int ax = 1; ax < 3; ax++) {
      double cur_ax_len = std::fabs(this->box.max_corner[ax] - this->box.min_corner[ax]);
      if (cur_ax_len > ax_len) {
        ax_len = cur_ax_len;
        ax_longest = ax;
      }
    }

    // Get the midpoint along the longest axis containing the given objects to
    // determine the left-right split
    double m = (1.0 / 2.0) * (this->box.min_corner[ax_longest] + this->box.max_corner[ax_longest]);

    // Now we need to partition <objects> into lists with lenth k and (N - k) surrounding midpoint
    std::vector<std::shared_ptr<Object>> objects_l;
    std::vector<std::shared_ptr<Object>> objects_r;       // a vector of pointers to objects

    for (std::shared_ptr<Object> obj_ptr : objects) {
      // All obj's have a Bounding Box <box> property
      if (obj_ptr->box.center()[ax_longest] < m) {
        /**
         * We put this object into the <objects_l>.
         * https://stackoverflow.com/questions/26860749/efficiency-of-c11-push-back-with-stdmove-versus-emplace-back-for-already
         * We are just inserting this shared pointer to obj into the new list.
         */
        objects_l.push_back(obj_ptr);
        // objects_l.emplace_back(obj_ptr);
      } else {
        // objects_r.emplace_back(obj_ptr);
        objects_r.push_back(obj_ptr);
      }
    }
    /**
       * Now, what if all the objects ended up on only one of the two sides of
       * the midpoint?
       */
      if (objects_l.size() > 0 && objects_r.size() == 0) {
        objects_r.push_back(objects_l.back());
        objects_l.pop_back();
      } else if (objects_l.size() == 0 && objects_r.size() > 0) {
        objects_l.push_back(objects_r.back());
        objects_r.pop_back();
      }

    // Now we recurse. README make_shared instead of new MyClass()
    this->left = std::make_shared<AABBTree>(objects_l, depth + 1);
    this->right = std::make_shared<AABBTree>(objects_r, depth + 1);

    // insert_box_into_box(this->left->box, this->box);
    // insert_box_into_box(this->right->box, this->box);
  }
  ////////////////////////////////////////////////////////////////////////////
}
