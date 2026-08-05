#include "Thread/PersistentWorker/persistent_worker.hpp"
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

PersistentWorker::PersistentWorker()
    : _thread("PersistenWorker", [this]() { workerLoop(); }), _running(true) {
  _thread.start();
}

PersistentWorker::~PersistentWorker() {
  _running = false;
  _thread.stop();
}

void PersistentWorker::addTask(const std::string& name,
                               const std::function<void()>& job) {
  std::lock_guard<std::mutex> lock(_mutex);
  _tasks[name] = job;
}

void PersistentWorker::removeTask(const std::string& name) {
  std::lock_guard<std::mutex> lock(_mutex);
  _tasks.erase(name);
}

void PersistentWorker::workerLoop() {

  while (_running) {
    std::unordered_map<std::string, std::function<void()>> tasksCopy;

    {
      std::lock_guard<std::mutex> lock(_mutex);
      tasksCopy = _tasks;
    }

    for (auto& [name, task] : tasksCopy) {
      if (task)
        task();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(10));
  }
}
