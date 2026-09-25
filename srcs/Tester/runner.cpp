#include "Tester/runner.hpp"

#include <iostream>

namespace Test {

void Runner::add(const std::string& suite, const std::string& name,
                 std::function<void()> function, const bool shouldFail) {
  tests.push_back({suite, name, function, shouldFail});
}

int Runner::run() {
  auto [time, res] = Timer::run([this] { return runTest(); });
  Printer::TestTime(time);

  return res;
}

int Runner::runTest() {
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

} // namespace Test
