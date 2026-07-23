#pragma once

#include "test_printer.hpp"

#include <functional>
#include <string>
#include <vector>

class TestRunner {
private:
  struct Test {
    std::string suite;
    std::string name;
    std::function<void()> function;
  };

  std::vector<Test> tests;

public:
  void add(const std::string& suite, const std::string& name,
           std::function<void()> function) {
    tests.push_back({suite, name, function});
  }

  int run() {
    int passed = 0;
    int failed = 0;

    for (auto& test : tests) {
      TestPrinter::OnTestStart(test.suite, test.name);

      bool success = true;

      try {
        test.function();
      } catch (const std::exception& e) {
        success = false;

        std::cout << "Exception: " << e.what() << '\n';
      } catch (...) {
        success = false;
      }

      if (success) {
        passed++;
      } else {
        failed++;
      }

      TestPrinter::OnTestEnd(test.suite, test.name, success);
    }

    TestPrinter::OnTestProgramEnd(tests.size(), passed, failed);

    return failed;
  }
};
