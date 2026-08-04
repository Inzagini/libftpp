#include "Thread/Thread/thread.hpp"

#include "../utils/test_runner.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

int main() {
  TestRunner runner;

  // THREAD DOES NOT START AUTOMATICALLY

  runner.add("Thread", "DoesNotStartAutomatically", []() {
    std::atomic<bool> executed = false;

    Thread thread("Worker", [&]() { executed = true; });

    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    assert(!executed);

    thread.start();
    thread.stop();

    assert(executed);
  });

  // START EXECUTES FUNCTION

  runner.add("Thread", "StartExecutesFunction", []() {
    std::atomic<bool> executed = false;

    Thread thread("Worker", [&]() { executed = true; });

    thread.start();
    thread.stop();

    assert(executed);
  });

  // STOP WAITS FOR THREAD

  runner.add("Thread", "StopJoinsThread", []() {
    std::atomic<bool> finished = false;

    Thread thread("Worker", [&]() {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      finished = true;
    });

    thread.start();
    thread.stop();

    assert(finished);
  });

  // FUNCTION RUNS EXACTLY ONCE

  runner.add("Thread", "RunsExactlyOnce", []() {
    std::atomic<int> counter = 0;

    Thread thread("Worker", [&]() { counter++; });

    thread.start();
    thread.stop();

    assert(counter == 1);
  });

  // MULTIPLE THREADS

  runner.add("Thread", "MultipleThreads", []() {
    std::atomic<int> counter = 0;

    Thread t1("Thread1", [&]() { counter++; });

    Thread t2("Thread2", [&]() { counter++; });

    Thread t3("Thread3", [&]() { counter++; });

    t1.start();
    t2.start();
    t3.start();

    t1.stop();
    t2.stop();
    t3.stop();

    assert(counter == 3);
  });

  // GET NAME

  runner.add("Thread", "GetName", []() {
    Thread thread("Physics", []() {});

    assert(thread.getName() == "Physics");
  });

  runner.add("Thread", "StartTwiceThrows", []() {
    Thread thread("Worker", []() {});

    thread.start();

    bool thrown = false;

    try {

      thread.start();

    } catch (...) {

      thrown = true;
    }

    thread.stop();

    assert(thrown);
  });

  return runner.run();
}
