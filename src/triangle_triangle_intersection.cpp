#include "triangle_triangle_intersection.h"
#include "ray_intersect_triangle.h"
#include "Ray.h"
#include <vector>

bool triangle_triangle_intersection(
  const Eigen::RowVector3d & A0,
  const Eigen::RowVector3d & A1,
  const Eigen::RowVector3d & A2,
  const Eigen::RowVector3d & B0,
  const Eigen::RowVector3d & B1,
  const Eigen::RowVector3d & B2)
{
  ////////////////////////////////////////////////////////////////////////////
  /**
   * After doing some research, I found this description
   * http://webee.technion.ac.il/~ayellet/Ps/TroppTalShimshoni.pdf
   * We are going to make each edge of triangle A into a ray, and check
   * it's intersection with triangle B as described in the study.
   */
  double t;

  // First we set up the rays
  Ray ray1 = Ray(A0, A1 - A0);
  Ray ray2 = Ray(A1, A0 - A1);
  Ray ray3 = Ray(A0, A2 - A0);
  Ray ray4 = Ray(A2, A0 - A2);
  Ray ray5 = Ray(A1, A2 - A1);
  Ray ray6 = Ray(A2, A1 - A2);

  std::vector<Ray> rays = { ray1, ray2, ray3, ray4, ray5, ray6 };
  for (auto &ray : rays) {
    if (ray_intersect_triangle(ray, B0, B1, B2, 0.0, 1.0, t)) {
      return true;
    }
  }


  return false;
  ////////////////////////////////////////////////////////////////////////////
}
