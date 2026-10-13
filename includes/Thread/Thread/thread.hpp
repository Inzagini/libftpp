#ifndef LIBFTPP_INCLUDES_THREAD_THREAD_THREAD_HPP
# define LIBFTPP_INCLUDES_THREAD_THREAD_THREAD_HPP

#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include <functional>
#include <thread>

class Thread {

public:
  Thread(const std::string& name, std::function<void()> funtion);
  ~Thread();
  Thread(const Thread&) = delete;
  Thread& operator=(const Thread&) = delete;
  Thread(Thread&&) noexcept = default;
  Thread& operator=(Thread&&) noexcept = default;

  void start();
  void stop();
  const std::string getName() const;

private:
  std::string _name;
  std::function<void()> _function;
  std::thread _thread;
  bool _started;
};
#endif // LIBFTPP_INCLUDES_THREAD_THREAD_THREAD_HPP
