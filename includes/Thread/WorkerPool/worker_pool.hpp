#pragma once

#include "Thread/threading.hpp"
#include <atomic>
#include <cstddef>
#include <exception>
#include <format>
#include <functional>
#include <stdexcept>
#include <thread>

class WorkerPool {

public:
  class IJobs {
  public:
    virtual ~IJobs() = default;
    virtual void execute() = 0;
  };

  explicit WorkerPool(std::size_t count);
  ~WorkerPool();
  WorkerPool(const WorkerPool&) = delete;
  WorkerPool& operator=(const WorkerPool&) = delete;

  void addJob(const std::function<void()>& job);
  void addJob(IJobs& job);

private:
  void workerLoop();

private:
  ThreadSafeQueue<std::function<void()>> _job;
  std::vector<Thread> _workders;
  std::atomic<bool> _running;
};
