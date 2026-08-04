#pragma once

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
