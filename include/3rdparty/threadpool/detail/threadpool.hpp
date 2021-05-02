
// add new work item to the pool
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) ->
  std::future<typename std::result_of<F(Args...)>::type> {
  using return_type = typename std::result_of<F(Args...)>::type;

  auto task = std::make_shared<std::packaged_task<return_type()>>(
    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

  std::future<return_type> res = task->get_future();
  {
    std::unique_lock<std::mutex> lock(mMutex);

    // don't allow enqueueing after stopping the pool
    if (mStop) {
      throw std::runtime_error("enqueue on stopped ThreadPool");
    }
    mTasks.emplace([task]() { (*task)(); });
  }
  mCondVar.notify_one();
  return res;
}
