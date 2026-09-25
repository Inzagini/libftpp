#pragma once

#include "Tester/printer.hpp"
#include "Timer/timer.hpp"

#include <functional>
#include <string>
#include <vector>

namespace Test {

class Runner {
private:
  struct Test {
    std::string suite;
    std::string name;
    std::function<void()> function;
    bool shouldFail;
  };

  std::vector<Test> tests;

public:
  void add(const std::string& suite, const std::string& name,
           std::function<void()> function, const bool shouldFail = false);

  int run();
  int runTest();
};

} // namespace Test
