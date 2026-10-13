#include "Chronometer/chronometer.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <chrono>
#include <thread>

void register_Chronometer(Test::Runner& runner) {

  runner.add("Chronometer", "ReturnsCallableResult", []() {
    auto [elapsed, result] = Chronometer::run([]() { return 42; });

    assert(result == 42);
    assert(elapsed >= std::chrono::steady_clock::duration::zero());
  });

  runner.add("Chronometer", "MeasuresElapsedTime", []() {
    auto [elapsed, result] = Chronometer::run([]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(20));
      return 1;
    });

    (void)result;
    assert(elapsed >= std::chrono::milliseconds(10));
  });
}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Chronometer(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif
