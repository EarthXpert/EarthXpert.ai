/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/gtest/gtest_main.h"
#include "3rdparty/taskflow/extaskflow.h"

#include <iostream>

//using namespace tf;

/**
 *  TestCase 0: 
 */
TEST(taskflow, ABCD) {
  tf::Executor executor;
  tf::Taskflow taskflow;

  auto [A, B, C, D] = taskflow.emplace(  // create 4 tasks
    []() { std::cout << "TaskA "; },
    []() { std::cout << "TaskB "; },
    []() { std::cout << "TaskC "; },
    []() { std::cout << "TaskD\n"; }
  );

  A.precede(B, C);  // A runs before B and C
  D.succeed(B, C);  // D runs after  B and C

  ASSERT_EQ(4, taskflow.num_tasks());

  executor.run(taskflow).wait();
}
