#include "AABBTree.h"

// See AABBTree.h for API
bool AABBTree::ray_intersect(
  const Ray& ray,
  const double min_t,
  const double max_t,
  double & t,
  std::shared_ptr<Object> & descendant) const
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * We are using the algorithm on page 303
   */
  bool hit = false;

  // First check if we even hit the outermost bounding box
  if (!ray_intersect_box(ray, this->box, min_t, max_t)) {
    return false;
  }

  // We've definitely hit a box, but not necessarily an object. Recurse
  double t_left, t_right;
  std::shared_ptr<Object> subtree_left = this->left;
  std::shared_ptr<Object> subtree_right = this->right;
  bool hit_left = (this->left != NULL) && this->left->ray_intersect(ray, min_t, max_t, t_left, subtree_left);
  bool hit_right = (this->right != NULL) && this->right->ray_intersect(ray, min_t, max_t, t_right, subtree_right);


  if (hit_left && hit_right) {
    // Then check for the closes hit
    if (t_left < t_right) {
      t = t_left;
      descendant = subtree_left;
    } else {
      t = t_right;
      descendant = subtree_right;
    }
  } else if (hit_left) {
    t = t_left;
    descendant = subtree_left;
  } else if (hit_right) {
    t = t_right;
    descendant = subtree_right;
  }

  if (hit_left || hit_right) {
    hit = true;
  }


  return hit;
  ////////////////////////////////////////////////////////////////////////////
}
