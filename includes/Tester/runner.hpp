
#pragma once

#include "Timer/timer.hpp"
#include "printer.hpp"

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
           std::function<void()> function, const bool shouldFail = false) {
    tests.push_back({suite, name, function, shouldFail});
  }

  int run() {
    auto [time, res] = Timer::run([this] { return runTest(); });
    Printer::TestTime(time);

    return res;
  }

  int runTest() {
    int passed = 0;
    int failed = 0;

    for (auto& test : tests) {
      Printer::OnTestStart(test.suite, test.name);

      bool success = true;

      try {
        test.function();
      } catch (const std::exception& e) {
        success = false;

        std::cout << "Exception: " << e.what() << '\n';
      } catch (...) {
        success = false;
      }

      if (test.shouldFail)
        success = !success;

      if (success) {
        passed++;
      } else {
        failed++;
      }

      Printer::OnTestEnd(test.suite, test.name, success);
    }

    Printer::OnTestProgramEnd(tests.size(), passed, failed);

    return failed;
  }
};
} // namespace Test
