#include "Tester/printer.hpp"

#include <iostream>

namespace Test {

void Printer::OnTestStart(const std::string& suite_name,
                          const std::string& test_name) {
  std::cout << BOLD << YELLOW << "[ RUN ] " << RESET << suite_name << "."
            << test_name << std::flush;
}

void Printer::OnTestEnd(const std::string& suite_name,
                        const std::string& test_name, bool passed) {
  std::cout << '\r';

  if (passed) {
    std::cout << BOLD << GREEN << "[ PASSED ] " << RESET;
  } else {
    std::cout << BOLD << RED << "[ FAILED ] " << RESET;
  }

  std::cout << suite_name << "." << test_name << std::endl;
}

void Printer::OnTestProgramEnd(int total_tests, int passed_tests,
                               int failed_tests) {
  std::cout << "\n=========================================\n";
  std::cout << "Total Tests : " << BOLD << CYAN << total_tests << RESET << '\n';
  std::cout << "Passed      : " << BOLD << GREEN << passed_tests << RESET
            << '\n';
  std::cout << "Failed      : " << BOLD << RED << failed_tests << RESET << '\n';
  std::cout << "=========================================\n";
}

void Printer::TestTime(const std::chrono::nanoseconds time) {
  std::cout << "Time: " << BOLD << YELLOW << time.count() << " ns" << RESET
            << "\n";
  std::cout << "=========================================\n";
}

} // namespace Test
