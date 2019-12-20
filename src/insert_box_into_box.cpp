#include "insert_box_into_box.h"

void insert_box_into_box(
  const BoundingBox & A,
  BoundingBox & B)
{
  ////////////////////////////////////////////////////////////////////////////
  // We're putting box A into box B -> Box B might need to grow!

  // Iterate over all the coordinates in R^3. See ipad
  for (int i = 0; i < 3; i++) {
    B.min_corner[i] = std::fmin(B.min_corner[i], A.min_corner[i]);
    B.max_corner[i] = std::fmax(B.max_corner[i], A.max_corner[i]);
  }
  ////////////////////////////////////////////////////////////////////////////
}
