#include "Tester/runner.hpp"
#include "Thread/threading.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

int main() {
  Test::Runner runner;

  /*
      BASIC JOB EXECUTION
  */

  runner.add("WorkerPool", "ExecuteSingleJob", []() {
    std::atomic<bool> executed = false;

    {
      WorkerPool pool(4);

      pool.addJob([&]() { executed = true; });

      while (!executed) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
      }
    }

    assert(executed);
  });

  /*
      MULTIPLE JOBS
  */

  runner.add("WorkerPool", "ExecuteMultipleJobs", []() {
    std::atomic<int> counter = 0;

    {
      WorkerPool pool(4);

      for (int i = 0; i < 10; i++) {
        pool.addJob([&]() { counter++; });
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    assert(counter == 10);
  });

  /*
      JOBS DISTRIBUTED BETWEEN WORKERS
  */

  runner.add("WorkerPool", "ConcurrentExecution", []() {
    std::atomic<int> running = 0;
    std::atomic<int> maxRunning = 0;

    {
      WorkerPool pool(4);

      for (int i = 0; i < 4; i++) {
        pool.addJob([&]() {
          int current = ++running;

          if (current > maxRunning) {
            maxRunning = current;
          }

          std::this_thread::sleep_for(std::chrono::milliseconds(50));

          --running;
        });
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    assert(maxRunning > 1);
  });

  /*
      JOB ORDER IS NOT GUARANTEED
  */

  runner.add("WorkerPool", "ExecuteAllJobs", []() {
    std::atomic<int> counter = 0;

    {
      WorkerPool pool(2);

      for (int i = 0; i < 100; i++) {
        pool.addJob([&]() { counter++; });
      }

      std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    assert(counter == 100);
  });

  /*
      STRING / CAPTURE TEST
  */

  runner.add("WorkerPool", "LambdaCapture", []() {
    std::string message;
    std::mutex mutex;

    {
      WorkerPool pool(1);

      pool.addJob([&]() {
        std::lock_guard<std::mutex> lock(mutex);

        message = "Hello WorkerPool";
      });

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    assert(message == "Hello WorkerPool");
  });

  /*
      EMPTY POOL SHUTDOWN
  */

  runner.add("WorkerPool", "EmptyPoolDestructor", []() {
    {
      WorkerPool pool(4);
    }

    assert(true);
  });

  return runner.run();
}
