#include <boost/algorithm/string/split.hpp>
#include <cassert>
#include <iostream>

/**
 *  \brief Maximum triangle path sum
 */
TEST(boost, MaxTriSum) {
  std::vector<int> numbers;

  std::string line;
  while (std::getline(std::cin, line)) {
    std::vector<std::string> toks;
    boost::split(toks, std::move(line), [](const char c) { return ' ' == c; });

    for (auto& tok : toks) {
      const int i = atoi(tok.c_str());
      numbers.push_back(i);
    }
  }

  const int h = numbers[0];  // triangle height
  const auto n = static_cast<int>(numbers.size()) - 1;  // number of triangle elements
  assert(h * (h + 1) == n * 2);

  int* const tri = numbers.data() + 1;  // start of triangle
  for (int i = h - 1; i > 0; --i) {
    const int ib = ((i - 1) * i) / 2; // begin of row i
    const int ie = (i * (i + 1)) / 2; // end

    for (int j = ib; j < ie; ++j) {
      const int k = j + i;  // first neighbor of next row

      tri[j] += std::max(tri[k], tri[k + 1]);
    }
  }

  const int maxSum = tri[0];
  std::cout << maxSum << std::endl;
}
