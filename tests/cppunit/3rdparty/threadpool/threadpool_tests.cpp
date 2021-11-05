/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/threadpool/threadpool.h"

#include <chrono>
#include <iostream>
#include <vector>

/**
 *  TestCase 0: 
 */
TEST(threadpool, ABCD) {
  ThreadPool pool;
  std::vector<std::future<int>> results;

  for (int i = 0; i < 8; ++i) {
    results.emplace_back(pool.enqueue([i] {
        std::cout << " hello " + std::to_string(i);
        //std::this_thread::sleep_for(std::chrono::milliseconds(1));
        std::cout << " world " + std::to_string(i);
        return i * i; 
      }));
  }

  std::cout << std::endl;
  for (auto&& result : results)
    std::cout << ", " << result.get();
  std::cout << std::endl;
}
