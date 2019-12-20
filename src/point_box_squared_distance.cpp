#include "point_box_squared_distance.h"
#include <algorithm>

double point_box_squared_distance(
  const Eigen::RowVector3d & query,
  const BoundingBox & box)
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * We will find the max distance between each coordinate of the box and point.
   * https://stackoverflow.com/questions/5254838/calculating-distance-between-a-point-and-a-rectangular-box-nearest-point
   */
  double dx, dy, dz;
  dx = std::max({box.min_corner[0] - query[0], 0.0, query[0] - box.max_corner[0]});
  dy = std::max({box.min_corner[1] - query[1], 0.0, query[1] - box.max_corner[1]});
  dz = std::max({box.min_corner[2] - query[2], 0.0, query[2] - box.max_corner[2]});


  return ((dx * dx) + (dy * dy) + (dz * dz));
  ////////////////////////////////////////////////////////////////////////////
}
