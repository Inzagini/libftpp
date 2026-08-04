#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include "Thread/threading.hpp"

Thread::Thread(const std::string& name, std::function<void()> function)
    : _name(name), _function(function), _started(false) {}

Thread::~Thread() { stop(); }

void Thread::start() {

  if (_started)
    throw std::runtime_error("Thread already started");

  _thread = std::thread([this]() {
    threadSafeCout.setPrefix(_name);
    _function();
  });

  _started = true;
}

void Thread::stop() {
  if (_thread.joinable())
    _thread.join();

  _started = false;
}

const std::string Thread::getName() const { return _name; }
