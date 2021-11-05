/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 *
 *  https://github.com/progschj/ThreadPool
 * -----------------------------------------------------------------------------
 */
#pragma once

#include "3rdparty/ex3rdparty.h"

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class EX3RDPARTY_EXPORTS ThreadPool {
public:
  explicit ThreadPool(unsigned numThreads = std::thread::hardware_concurrency());
  ~ThreadPool();

  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;

private:
  std::vector<std::thread> mWorkers;  // track numThreads to join them
  std::queue<std::function<void()>> mTasks;  // the task queue

  // synchronization
  std::mutex mMutex;
  std::condition_variable mCondVar;
  bool mStop{false};
};

#include "detail/threadpool.hpp"
