#pragma once

#include <iostream>
#include <string>

class TestPrinter {
public:
  static void OnTestStart(const std::string& suite_name,
                          const std::string& test_name) {
    std::cout << "[ RUN      ] " << suite_name << "." << test_name << std::endl;
  }

  static void OnTestEnd(const std::string& suite_name,
                        const std::string& test_name, bool passed) {
    if (passed) {
      std::cout << "[  PASSED  ] ";
    } else {
      std::cout << "[  FAILED  ] ";
    }

    std::cout << suite_name << "." << test_name << std::endl;
  }

  static void OnTestProgramEnd(int total_tests, int passed_tests,
                               int failed_tests) {
    std::cout << "\n=========================================\n";
    std::cout << "Total Tests : " << total_tests << '\n';
    std::cout << "Passed      : " << passed_tests << '\n';
    std::cout << "Failed      : " << failed_tests << '\n';
    std::cout << "=========================================\n";
  }
};
