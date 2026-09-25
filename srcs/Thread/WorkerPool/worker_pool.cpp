#include "Thread/WorkerPool/worker_pool.hpp"
#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include <stdexcept>

WorkerPool::WorkerPool(std::size_t count) {

  _running = true;
  _workders.reserve(count);

  for (std::size_t i{}; i < count; i++) {
    _workders.emplace_back(std::format("Worker-{} ", i),
                           [this]() { workerLoop(); });

    _workders.back().start();
  }
}

WorkerPool::~WorkerPool() {
  _running = false;
  for (auto& worker : _workders)
    worker.stop();
}

void WorkerPool::addJob(const std::function<void()>& job) {
  _job.push_back(job);
}

void WorkerPool::addJob(IJobs& job) {
  addJob([&job]() { job.execute(); });
}

void WorkerPool::workerLoop() {
  while (_running) {

    try {

      auto job = _job.pop_front();

      if (job)
        job();

    } catch (const std::runtime_error&) {
      std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
  }
}
