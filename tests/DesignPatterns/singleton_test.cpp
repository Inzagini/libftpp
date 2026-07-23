#include "DesignPatterns/Singleton/singleton.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <string>

/*
    TEST SINGLETON CLASS
*/

class Logger {

  friend class Singleton<Logger>;

private:
  int value;
  std::string name;

  Logger(int v, const std::string& n) : value(v), name(n) {}

public:
  int getValue() const { return value; }

  const std::string& getName() const { return name; }
};

/*
    SECOND SINGLETON TYPE
*/

class Counter {

  friend class Singleton<Counter>;

private:
  int count;

  Counter(int c) : count(c) {}

public:
  int getCount() const { return count; }
};

int main() {
  TestRunner runner;

  /*
      BASIC INSTANTIATION
  */

  runner.add("Singleton", "CreateInstance", []() {
    Singleton<Logger> manager;

    manager.instantiate(42, "MainLogger");

    Logger* logger = manager.instance();

    assert(logger != nullptr);
    assert(logger->getValue() == 42);
    assert(logger->getName() == "MainLogger");

    manager.destroy();
  });

  /*
      SAME INSTANCE
  */

  runner.add("Singleton", "SameInstanceReturned", []() {
    Singleton<Logger> manager;

    manager.instantiate(10, "Test");

    Logger* first = manager.instance();

    Logger* second = manager.instance();

    assert(first == second);

    manager.destroy();
  });

  /*
      DOUBLE INSTANTIATION SHOULD FAIL
  */

  runner.add("Singleton", "MultipleInstantiationThrows", []() {
    Singleton<Logger> manager;

    manager.instantiate(1, "First");

    bool thrown = false;

    try {
      manager.instantiate(2, "Second");
    } catch (const std::runtime_error&) {
      thrown = true;
    }

    assert(thrown);

    manager.destroy();
  });

  /*
      DESTROY AND RECREATE
  */

  runner.add("Singleton", "DestroyAndRecreate", []() {
    Singleton<Logger> manager;

    manager.instantiate(100, "Old");

    assert(manager.instance()->getValue() == 100);

    manager.destroy();

    manager.instantiate(200, "New");

    assert(manager.instance()->getValue() == 200);

    manager.destroy();
  });

  /*
      INSTANCE BEFORE CREATION
  */

  runner.add("Singleton", "InstanceBeforeCreation", []() {
    Singleton<Logger> manager;

    Logger* logger = manager.instance();

    assert(logger == nullptr);
  });

  /*
      DIFFERENT SINGLETON TYPES
  */

  runner.add("Singleton", "DifferentTypesIndependent", []() {
    Singleton<Logger> loggerManager;

    Singleton<Counter> counterManager;

    loggerManager.instantiate(5, "Logger");

    counterManager.instantiate(99);

    assert(loggerManager.instance()->getValue() == 5);

    assert(counterManager.instance()->getCount() == 99);

    loggerManager.destroy();

    counterManager.destroy();
  });

  return runner.run();
}
