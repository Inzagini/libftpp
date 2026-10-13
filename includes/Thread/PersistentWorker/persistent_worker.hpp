#ifndef LIBFTPP_INCLUDES_THREAD_PERSISTENTWORKER_PERSISTENT_WORKER_HPP
# define LIBFTPP_INCLUDES_THREAD_PERSISTENTWORKER_PERSISTENT_WORKER_HPP

#include "Thread/Thread/thread.hpp"
#include "Thread/threading.hpp"
#include <atomic>
#include <chrono>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

class PersistentWorker {

public:
  PersistentWorker();
  ~PersistentWorker();
  PersistentWorker(const PersistentWorker&) = delete;
  PersistentWorker& operator=(const PersistentWorker&) = delete;

  void addTask(const std::string& name, const std::function<void()>& job);
  void removeTask(const std::string& name);

private:
  void workerLoop();

private:
  Thread _thread;
  std::unordered_map<std::string, std::function<void()>> _tasks;
  std::mutex _mutex;
  std::atomic<bool> _running;
};
#endif // LIBFTPP_INCLUDES_THREAD_PERSISTENTWORKER_PERSISTENT_WORKER_HPP
