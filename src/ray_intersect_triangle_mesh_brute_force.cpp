#include "ray_intersect_triangle_mesh_brute_force.h"
#include "ray_intersect_triangle.h"

bool ray_intersect_triangle_mesh_brute_force(
  const Ray & ray,
  const Eigen::MatrixXd & V,
  const Eigen::MatrixXi & F,
  const double min_t,
  const double max_t,
  double & hit_t,
  int & hit_f)
{
  ////////////////////////////////////////////////////////////////////////////
  bool hit = false;
  double t_min = std::numeric_limits<double>::infinity();
  double t_prime;
  int f_hit_idx;
  // Eigen::RowVector3i F_row_i;
  Eigen::RowVector3d A, B, C;

  // Iterate over the rows of F
  for (int i = 0; i < F.rows(); i++) {
    // Each row of F is a ROW index into V. First get row <i> of indices from F
    // F_row_i = F.row(i);

    // Each row of V is a vertex point in R^3.
    // A = V.row(F_row_i[0]);
    // B = V.row(F_row_i[1]);
    // C = V.row(F_row_i[2]);
    A = V.row(F(i, 0));
    B = V.row(F(i, 1));
    C = V.row(F(i, 2));

    // Check if we got a hit
    if (ray_intersect_triangle(ray, A, B, C, min_t, max_t, t_prime)) {
      if (t_prime < t_min) {
        t_min = t_prime;
        f_hit_idx = i;
        hit = true;
      }
    }
  }

  if (hit) {
    hit_t = t_min;
    hit_f = f_hit_idx;
  }


  return hit;
  ////////////////////////////////////////////////////////////////////////////
}
