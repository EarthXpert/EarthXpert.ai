/* -----------------------------------------------------------------------------
 *  Copyright (c) 2019.  All rights reserved.
 * -----------------------------------------------------------------------------
 */
#include "3rdparty/threadpool/threadpool.h"

#include <algorithm>

 // the constructor just launches some amount of mWorkers
ThreadPool::ThreadPool(unsigned numThreads) {
  if (0 == numThreads)
    numThreads = std::max(4u, std::thread::hardware_concurrency());
  for (unsigned i = 0; i < numThreads; ++i) {
    mWorkers.emplace_back([this] {
      for (;;) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(mMutex);
          mCondVar.wait(lock, [this] {
            return mStop || !mTasks.empty(); });
          if (mStop && mTasks.empty())
            return;
          task = std::move(mTasks.front());
          mTasks.pop();
        }

        task();
      } });
  }
}

// the destructor joins all numThreads
ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mMutex);
    mStop = true;
  }
  mCondVar.notify_all();
  for (auto& worker : mWorkers)
    worker.join();
}
