#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

/**
 *  \brief Maximum triangle path sum
 */
TEST(stl, MaxTriSum) {
  std::string line;
  std::vector<int> numbers;

  while (std::getline(std::cin, line)) {
    char* tok = strtok(line.data(), " ");
    while (tok) {
      const int i = atoi(tok);
      numbers.push_back(i);

      tok = strtok(nullptr, " ");
    }
  }

  const int h = numbers[0];  // triangle height
  const auto n = static_cast<int>(numbers.size()) - 1;  // number of triangle elements
  assert(h * (h + 1) == n * 2);

  int* tri = numbers.data() + 1;  // start of triangle
  for (int i = h - 1; i > 0; --i) {
    const int rb = (i * (i - 1)) / 2;
    const int re = (i * (i + 1)) / 2;
    for (int j = rb; j < re; ++j) {
      const int k = i + j;
      tri[j] += std::max(tri[k], tri[k + 1]);
    }
  }

  const int maxSum = tri[0];
  std::cout << maxSum << std::endl;
}
