#include "IOStream/Log/log.hpp"
#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

extern thread_local ThreadSafeIOStream threadSafeCout;

class CoutCapture {
public:
  void start() { old = std::cout.rdbuf(buffer.rdbuf()); }

  std::string stop() {
    std::cout.rdbuf(old);
    return buffer.str();
  }

private:
  std::stringstream buffer;
  std::streambuf* old;
};

int main() {
  Test::Runner runner;

  runner.add("Log", "LevelAccessor", []() {
    Log::setLevel(Log::Level::Debug);
    assert(Log::level() == Log::Level::Debug);

    Log::setLevel(Log::Level::Info);
    assert(Log::level() == Log::Level::Info);
  });

  runner.add("Log", "FormatsArguments", []() {
    CoutCapture capture;
    capture.start();

    Log::setLevel(Log::Level::Debug);
    Log::info("hello ", 42);

    assert(capture.stop() == "[INFO] hello 42\n");
  });

  runner.add("Log", "AllLevels", []() {
    CoutCapture capture;
    capture.start();

    Log::setLevel(Log::Level::Debug);
    Log::debug("a");
    Log::info("b");
    Log::warning("c");
    Log::error("d");

    assert(capture.stop() ==
           "[DEBUG] a\n[INFO] b\n[WARNING] c\n[ERROR] d\n");
  });

  runner.add("Log", "FiltersBelowLevel", []() {
    CoutCapture capture;
    capture.start();

    Log::setLevel(Log::Level::Warning);
    Log::debug("d");
    Log::info("i");
    Log::warning("w");
    Log::error("e");

    assert(capture.stop() == "[WARNING] w\n[ERROR] e\n");
  });

  return runner.run();
}
