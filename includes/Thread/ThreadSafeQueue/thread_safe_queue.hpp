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

template <typename TType>
void ThreadSafeQueue<TType>::push_back(const TType& newElement) {
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_back(newElement);
  }
  _condition.notify_one();
}

template <typename TType>
void ThreadSafeQueue<TType>::push_front(const TType& newElement) {
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _queue.push_front(newElement);
  }

  _condition.notify_one();
}

template <typename TType> TType ThreadSafeQueue<TType>::pop_back() {
  std::lock_guard<std::mutex> lock(_mutex);

  if (_queue.empty())
    throw std::runtime_error("Queue is empty");

  TType element = _queue.back();
  _queue.pop_back();

  return element;
}

template <typename TType> TType ThreadSafeQueue<TType>::pop_front() {
  std::lock_guard<std::mutex> lock(_mutex);

  if (_queue.empty())
    throw std::runtime_error("Queue is empty");

  TType elemet = _queue.front();
  _queue.pop_front();

  return elemet;
}

template <typename TType> bool ThreadSafeQueue<TType>::empty() const {
  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.empty();
}

template <typename TType> size_t ThreadSafeQueue<TType>::size() const {

  std::lock_guard<std::mutex> lock(_mutex);
  return _queue.size();
}

template <typename TType> void ThreadSafeQueue<TType>::stop() {
  {
    std::lock_guard<std::mutex> lock(_mutex);
    _stopped = true;
  }

  _condition.notify_all();
}

template <typename TType> TType ThreadSafeQueue<TType>::wait_pop_front() {

  std::unique_lock<std::mutex> lock(_mutex);

  _condition.wait(lock, [this] { return _stopped || !_queue.empty(); });

  if (_stopped && _queue.empty())
    throw std::runtime_error("Queue stopped");

  auto element = _queue.front();
  _queue.pop_front();

  return element;
}
