/* -----------------------------------------------------------------------------
 *  Copyright (c) 2018.  All rights reserved.
 *
 *  https://github.com/progschj/ThreadPool
 * -----------------------------------------------------------------------------
 */
#pragma once

#include <condition_variable>
#include <functional>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <stdexcept>
#include <thread>
#include <vector>

class ThreadPool {
public:
  explicit ThreadPool(size_t numThreads = std::thread::hardware_concurrency());
  ~ThreadPool();

  template<class F, class... Args>
  auto enqueue(F&& f, Args&&... args)
    ->std::future<typename std::result_of<F(Args...)>::type>;

private:
  std::vector<std::thread> mWorkers;  // track numThreads to join them
  std::queue<std::function<void()>> mTasks;  // the task queue

  // synchronization
  std::mutex mQueueMtx;
  std::condition_variable mCondVar;
  bool mStop;
};
 
// the constructor just launches some amount of mWorkers
inline ThreadPool::ThreadPool(size_t numThreads)
  : mStop(false) {
  if (0 == numThreads)
    numThreads = 4;
  for (size_t i = 0; i < numThreads; ++i) {
    mWorkers.emplace_back([this] {
      for (;;) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(mQueueMtx);
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

// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args)
-> std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mQueueMtx);

    // don't allow enqueueing after stopping the pool
    if (mStop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    mTasks.emplace([task]() { (*task)(); });
  }
  mCondVar.notify_one();
  return res;
}

// the destructor joins all numThreads
inline ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mQueueMtx);
    mStop = true;
  }
  mCondVar.notify_all();
  for (auto& worker : mWorkers)
    worker.join();
}
