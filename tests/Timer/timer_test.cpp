#include "Timer/timer.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <chrono>
#include <thread>

void register_Timer(Test::Runner& runner) {

  runner.add("Timer", "NotTimedOutBeforeDuration", []() {
    Timer timer(std::chrono::milliseconds(50));

    assert(!timer.timedOut());
    assert(!timer.expired());
  });

  runner.add("Timer", "TimesOutAfterDuration", []() {
    Timer timer(std::chrono::milliseconds(10));

    std::this_thread::sleep_for(std::chrono::milliseconds(25));

    assert(timer.timedOut());
  });

  runner.add("Timer", "ResetRestartsCountdown", []() {
    Timer timer(std::chrono::milliseconds(15));

    std::this_thread::sleep_for(std::chrono::milliseconds(25));
    assert(timer.timedOut());

    timer.reset();
    assert(!timer.timedOut());
  });

  runner.add("Timer", "SetDurationRestartsCountdown", []() {
    Timer timer;

    timer.setDuration(std::chrono::milliseconds(100));

    assert(!timer.timedOut());
    assert(timer.remaining() <= std::chrono::milliseconds(100));
  });
}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Timer(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif
