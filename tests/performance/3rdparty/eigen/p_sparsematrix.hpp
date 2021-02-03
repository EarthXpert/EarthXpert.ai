/* -----------------------------------------------------------------------------
 *  Copyright (c) 2020.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include <Eigen/Sparse>

using namespace earthxpert;
using namespace::Eigen;

static constexpr int64_t kN = 1'000'000;

enum class FillOrder {
  eFO_ByRow,
  eFO_ByCol
};

inline std::pair<int, int> IndexToRowCol(const int64_t index) {
  return { int(index / kN), int(index % kN) };
}

template<StorageOptions option = RowMajor, FillOrder fillOrder = FillOrder::eFO_ByCol>
void Insert(const int64_t count) {
  SparseMatrix<double, option> A(kN, kN);

  std::vector<int> reserveSizes(kN, (count - 1) / kN + 1);
  A.reserve(reserveSizes);

  int r, c;
  for (int64_t i = 0; i < count; ++i) {
    std::tie(c, r) = IndexToRowCol(i);
    if constexpr (FillOrder::eFO_ByRow == fillOrder)
      std::swap(r, c);
    A.insert(r, c) = 0.1;
  }
}

//  \todo iterator to Triplet generator
template<StorageOptions option = RowMajor, FillOrder fillOrder = FillOrder::eFO_ByCol>
void SetTriplets(const int64_t count) {
  std::vector<Triplet<double>> egTri;
  egTri.reserve(count);

  int r, c;
  for (int64_t i = 0; i < count; ++i) {
    std::tie(c, r) = IndexToRowCol(i);
    if constexpr (FillOrder::eFO_ByRow == fillOrder)
      std::swap(r, c);
    egTri.emplace_back(r, c, 0.1);
  }

  SparseMatrix<double, option> A(kN, kN);
  A.setFromTriplets(egTri.begin(), egTri.end());
}

/**
 *  TestCase 1: Insert()
 */
void SpMatDR_Insert_ByCol(const int64_t& count) {
  Insert<RowMajor, FillOrder::eFO_ByCol>(count);
}

void SpMatDC_Insert_ByRow(const int64_t& count) {
  Insert<ColMajor, FillOrder::eFO_ByRow>(count);
}

/**
 *  TestCase 2: SetTriplets()
 */
void SpMatDR_SetTriplets_ByCol(const int64_t& count) {
  SetTriplets<RowMajor, FillOrder::eFO_ByCol>(count);
}

void SpMatDC_SetTriplets_ByRow(const int64_t& count) {
  SetTriplets<ColMajor, FillOrder::eFO_ByRow>(count);
}

/**
 *  \remark reserve() uses compressed mode that has bottlenecks
 *  at 4x+3 ~ 5x, 10x+3 ~ 11x, 22x+3 ~ 23x, etc.
 */
//static const std::vector<int64_t> args_sp{ kN, kN * 4, kN * 4 + 3,
//  kN * 10, kN * 10 + 3, kN * 22, kN * 22 + 3 }; // kN*46+3, kN*94+3
static const std::vector<int64_t> args_sp{ kN, kN * 4, kN * 10 };

SLTBENCH_FUNCTION_WITH_ARGS(SpMatDR_Insert_ByCol, args_sp);
SLTBENCH_FUNCTION_WITH_ARGS(SpMatDC_Insert_ByRow, args_sp);
SLTBENCH_FUNCTION_WITH_ARGS(SpMatDR_SetTriplets_ByCol, args_sp);
SLTBENCH_FUNCTION_WITH_ARGS(SpMatDC_SetTriplets_ByRow, args_sp);
