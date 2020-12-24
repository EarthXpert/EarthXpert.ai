#include <boost/multi_array.hpp>

// Create a 3D array that is 3 x 4 x 2
TEST(boost, multiarray) {
  using array3d = boost::multi_array<double, 3>;
  using index = array3d::index;

  array3d A(boost::extents[3][4][2]);

  // Assign values to the elements
  int values = 0;
  for (index i = 0; i != 3; ++i)
    for (index j = 0; j != 4; ++j)
      for (index k = 0; k != 2; ++k)
        A[i][j][k] = values++;

  // Verify values
  int verify = 0;
  for (index i = 0; i != 3; ++i)
    for (index j = 0; j != 4; ++j)
      for (index k = 0; k != 2; ++k)
        ASSERT_EQ(A[i][j][k], verify++);
}
