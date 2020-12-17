#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <iostream>

/**
 *  \brief Decode name
 */
TEST(stl, DecodeName) {
  std::string line;
  while (std::getline(std::cin, line)) {
    std::string result;

    char* keys = line.data() + line.find('|') + 1;
    char* tok = strtok(keys, " ");
    while (tok) {
      const int i = atoi(tok) - 1;
      result.append(1, line[i]);

      tok = strtok(nullptr, " ");
    }
  
    std::cout << result << std::endl;
  }
}

/**
 *  \brief Maximum triangle path sum
 */
TEST(stl, MaxTriSum) {
  std::vector<int> numbers;

  std::string line;
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
