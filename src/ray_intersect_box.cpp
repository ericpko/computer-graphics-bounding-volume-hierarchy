#include "ray_intersect_box.h"
#include <iostream>

bool ray_intersect_box(
  const Ray & ray,
  const BoundingBox& box,
  const double min_t,
  const double max_t)
{
  ////////////////////////////////////////////////////////////////////////////
  bool hit = false;

  // We test on the reciprocal of the direction (pg 302)
  double x_d = 1 / ray.direction[0];
  double y_d = 1 / ray.direction[1];
  double z_d = 1 / ray.direction[2];

  double x_e = ray.origin[0];
  double y_e = ray.origin[1];
  double z_e = ray.origin[2];

  double t_xmin, t_xmax, t_ymin, t_ymax, t_zmin, t_zmax;

  double x_min = box.min_corner[0];
  double x_max = box.max_corner[0];
  double y_min = box.min_corner[1];
  double y_max = box.max_corner[1];
  double z_min = box.min_corner[2];
  double z_max = box.max_corner[2];

  if (x_d >= 0) {
    t_xmin = x_d * (x_min - x_e);
    t_xmax = x_d * (x_max - x_e);
  } else {
    t_xmin = x_d * (x_max - x_e);
    t_xmax = x_d * (x_min - x_e);
  }

  if (y_d >= 0) {
    t_ymin = y_d * (y_min - y_e);
    t_ymax = y_d * (y_max - y_e);
  } else {
    t_ymin = y_d * (y_max - y_e);
    t_ymax = y_d * (y_min - y_e);
  }

  if (z_d >= 0) {
    t_zmin = z_d * (z_min - z_e);
    t_zmax = z_d * (z_max - z_e);
  } else {
    t_zmin = z_d * (z_max - z_e);
    t_zmax = z_d * (z_min - z_e);
  }

  /**
   * Now we check if the ray intersected the bounding box
   * Find the maximum of all the min points and the minimum of all the max points.
   * See ipad.
   */
  if (std::max({t_xmin, t_ymin, t_xmin}) < std::min({t_xmax, t_ymax, t_zmax})) {
    hit = true;
  }


  return hit;
  ////////////////////////////////////////////////////////////////////////////
}
