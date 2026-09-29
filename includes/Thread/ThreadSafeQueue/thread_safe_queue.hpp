#pragma once

#include <condition_variable>
#include <deque>
#include <mutex>
#include <stdexcept>
#include <thread>

template <typename TType> class ThreadSafeQueue {

public:
  ThreadSafeQueue() = default;
  ~ThreadSafeQueue() = default;
  ThreadSafeQueue(const ThreadSafeQueue&) = delete;
  ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

  void push_back(const TType& newElement);
  void push_front(const TType& newElement);
  TType pop_back();
  TType pop_front();
  TType wait_pop_front();

  bool empty() const;
  void stop();

  std::size_t size() const;

private:
  std::deque<TType> _queue;
  mutable std::mutex _mutex;
  std::condition_variable _condition;
  bool _stopped = false;
};

#include "thread_safe_queue.tpp"
