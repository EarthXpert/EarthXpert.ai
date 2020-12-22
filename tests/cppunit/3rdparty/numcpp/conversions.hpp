#include <Eigen/Dense>

#include <iostream>
#include <numeric>

using EigenIntMatrix = Eigen::Matrix<int, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using EigenIntMatrixMap = Eigen::Map<EigenIntMatrix>;

TEST(numcpp, Eigen3) {
  // construct some NumCpp arrays
  auto ncA = nc::random::randInt<int>({ 5, 5 }, 0, 10);
  auto ncB = nc::random::randInt<int>({ 5, 5 }, 0, 10);

  std::cout << "ncA:\n" << ncA << std::endl;
  std::cout << "ncB:\n" << ncB << std::endl;

  // map the arrays to Eigen 
  auto eigenA = EigenIntMatrixMap(ncA.data(), ncA.numRows(), ncA.numCols());
  auto eigenB = EigenIntMatrixMap(ncB.data(), ncB.numRows(), ncB.numCols());

  auto eigenC = eigenA + eigenB;  // add the two Eigen matrices
  auto ncC = ncA + ncB;  // add the two NumCpp arrays for a sanity check

  // convert the Eigen result back to NumCpp
  int* dataPtr = new int[eigenC.rows() * eigenC.cols()];
  EigenIntMatrixMap(dataPtr, eigenC.rows(), eigenC.cols()) = eigenC;
  auto ncCeigen = nc::NdArray<int>(dataPtr, eigenC.rows(), eigenC.cols(), true);

  ASSERT_TRUE(nc::array_equal(ncC, ncCeigen));
}

#include "opencv2/core/core.hpp"

constexpr nc::uint32 NUM_ROWS = 512;
constexpr nc::uint32 NUM_COLS = 512;

TEST(numcpp, OpenCV) {
  // create a ramp image with NumCpp
  constexpr auto numHalfCols = NUM_COLS / 2; // integer division
  auto ncArray = nc::NdArray<nc::uint8>(NUM_ROWS, NUM_COLS);
  for (nc::uint32 row = 0; row < ncArray.numRows(); ++row) {
    auto begin = ncArray.begin(row);
    std::iota(begin, begin + numHalfCols, nc::uint8{ 0 });

    auto rbegin = ncArray.rbegin(row);
    std::iota(rbegin, rbegin + numHalfCols, nc::uint8{ 0 });
  }

  // convert to OpenCV Mat
  cv::Mat cvArray(ncArray.numRows(), ncArray.numCols(), CV_8SC1, ncArray.data());

  // tranpose the Mat with OpenCV
  auto transposedCvArray = cv::Mat(cvArray.cols, cvArray.rows, CV_8SC1);
  cv::transpose(cvArray, transposedCvArray);

  // convert the transposed OpenCV Mat to a NumCpp array
  nc::NdArray<nc::uint8> transposedNcArray(transposedCvArray.data, transposedCvArray.rows, transposedCvArray.cols);

  ASSERT_TRUE(nc::array_equal(transposedNcArray, ncArray.transpose()));
}

TEST(numcpp, NumPy) {
}
