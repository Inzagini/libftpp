#include "../utils/test_runner.hpp"
#include "Thread/PersistentWorker/persistent_worker.hpp"

#include <atomic>
#include <cassert>
#include <chrono>
#include <thread>

int main() {
  TestRunner runner;

  /*
      SINGLE TASK EXECUTION
  */

  runner.add("PersistentWorker", "ExecuteSingleTask", []() {
    std::atomic<int> counter = 0;

    {
      PersistentWorker worker;

      worker.addTask("Counter", [&]() { counter++; });

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    assert(counter > 0);
  });

  /*
      MULTIPLE TASKS
  */

  runner.add("PersistentWorker", "ExecuteMultipleTasks", []() {
    std::atomic<int> taskA = 0;
    std::atomic<int> taskB = 0;

    {
      PersistentWorker worker;

      worker.addTask("TaskA", [&]() { taskA++; });

      worker.addTask("TaskB", [&]() { taskB++; });

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    assert(taskA > 0);
    assert(taskB > 0);
  });

  /*
      REMOVE TASK
  */

  runner.add("PersistentWorker", "RemoveTask", []() {
    std::atomic<int> counter = 0;

    {
      PersistentWorker worker;

      worker.addTask("Counter", [&]() { counter++; });

      std::this_thread::sleep_for(std::chrono::milliseconds(50));

      worker.removeTask("Counter");

      int valueAfterRemove = counter;

      std::this_thread::sleep_for(std::chrono::milliseconds(50));

      assert(counter == valueAfterRemove);
    }
  });

  /*
      ADD TASK AFTER START
  */

  runner.add("PersistentWorker", "AddTaskLater", []() {
    std::atomic<bool> executed = false;

    {
      PersistentWorker worker;

      std::this_thread::sleep_for(std::chrono::milliseconds(50));

      worker.addTask("LateTask", [&]() { executed = true; });

      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    assert(executed);
  });

  /*
      TASK NAME REPLACEMENT
  */

  runner.add("PersistentWorker", "ReplaceExistingTask", []() {
    std::atomic<int> first = 0;
    std::atomic<int> second = 0;

    {
      PersistentWorker worker;

      worker.addTask("SameName", [&]() { first++; });

      std::this_thread::sleep_for(std::chrono::milliseconds(50));

      worker.addTask("SameName", [&]() { second++; });

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    assert(second > 0);
  });

  /*
      EMPTY WORKER DESTRUCTOR
  */

  runner.add("PersistentWorker", "EmptyDestructor", []() {
    {
      PersistentWorker worker;
    }

    assert(true);
  });

  return runner.run();
}
