#pragma once

#include <iostream>
#include <string>

class TestPrinter {

private:
  // ANSI color codes
  static constexpr const char* RESET = "\033[0m";
  static constexpr const char* YELLOW = "\033[33m";
  static constexpr const char* GREEN = "\033[32m";
  static constexpr const char* RED = "\033[31m";
  static constexpr const char* CYAN = "\033[36m";

public:
  static void OnTestStart(const std::string& suite_name,
                          const std::string& test_name) {
    std::cout << YELLOW << "[   RUN    ] " << RESET << suite_name << "."
              << test_name << std::endl;
  }

  static void OnTestEnd(const std::string& suite_name,
                        const std::string& test_name, bool passed) {
    if (passed) {
      std::cout << GREEN << "[  PASSED  ] " << RESET;
    } else {
      std::cout << RED << "[  FAILED  ] " << RESET;
    }

    std::cout << suite_name << "." << test_name << std::endl;
  }

  static void OnTestProgramEnd(int total_tests, int passed_tests,
                               int failed_tests) {
    std::cout << "\n=========================================\n";
    std::cout << "Total Tests : " << CYAN << total_tests << RESET << '\n';
    std::cout << "Passed      : " << GREEN << passed_tests << RESET << '\n';
    std::cout << "Failed      : " << RED << failed_tests << RESET << '\n';
    std::cout << "=========================================\n";
  }
};
