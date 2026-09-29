#include "Tester/printer.hpp"

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
  std::cout << "Time: " << BOLD << YELLOW << formatTime(time) << RESET << "\n";
  std::cout << "=========================================\n";
}

std::string Printer::formatTime(const std::chrono::nanoseconds time) {
  static constexpr const char* units[] = {"ns", "µs", "ms", "s"};
  constexpr std::size_t unitCount = sizeof(units) / sizeof(units[0]);

  double value = static_cast<double>(time.count());
  std::size_t unit = 0;

  while (value >= 10000.0 && unit + 1 < unitCount) {
    value /= 1000.0;
    ++unit;
  }

  std::ostringstream stream;
  stream << value << " " << units[unit];

  return stream.str();
}

} // namespace Test
