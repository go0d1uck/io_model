#pragma once

#include <condition_variable>
#include <cstddef>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
  std::mutex _mutex_task_queue;
  std::condition_variable _condition_wake_up;
  std::vector<std::thread> _thread_pool;
  std::queue<std::function<void()>> _task_queue;
  bool _stop;

 public:
  // 构造函数 constructor
  ThreadPool(size_t thread_num) : _stop(false) {
    for (size_t i = 0; i < thread_num; ++i) {
      _thread_pool.emplace_back([this] {
        for (;;) {
          std::function<void()> task;
          {
            std::unique_lock<std::mutex> lock(_mutex_task_queue);
            _condition_wake_up.wait(
                lock, [this] { return _stop || !_task_queue.empty(); });
            // 没有任务需要处理且需要停止，那么结束线程
            if (_stop && _task_queue.empty()) {
              return;
            }
            // 从队列里面取出任务
            task = std::move(_task_queue.front());
            _task_queue.pop();
          }
          task();
        }
      });
    }
  }
  // 析构函数 destructor
  ~ThreadPool() {
    {
      std::unique_lock<std::mutex> lock(_mutex_task_queue);
      _stop = true;
    }
    _condition_wake_up.notify_all();
    for (auto& thread : _thread_pool) thread.join();
  }

  template <class F, class... Args>
  void enqueue(F&& f, Args&&... args) {
    {
      std::unique_lock<std::mutex> lock(_mutex_task_queue);
      _task_queue.emplace([=] { f(args...); });
    }
    _condition_wake_up.notify_one();
  }
};