#pragma once

#include <chrono>
#include <string>

namespace Test {

class Printer {

private:
  // ANSI color codes
  static constexpr const char* BOLD = "\033[1m";
  static constexpr const char* RESET = "\033[0m";
  static constexpr const char* YELLOW = "\033[33m";
  static constexpr const char* GREEN = "\033[32m";
  static constexpr const char* RED = "\033[31m";
  static constexpr const char* CYAN = "\033[36m";

public:
  static void OnTestStart(const std::string& suite_name,
                          const std::string& test_name);

  static void OnTestEnd(const std::string& suite_name,
                        const std::string& test_name, bool passed);

  static void OnTestProgramEnd(int total_tests, int passed_tests,
                               int failed_tests);

  static void TestTime(const std::chrono::nanoseconds time);
};

} // namespace Test
