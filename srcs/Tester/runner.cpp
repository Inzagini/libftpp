#include "Tester/runner.hpp"

#include <algorithm>

namespace Test {

void Runner::add(const std::string& suite, const std::string& name,
                 std::function<void()> function, const bool shouldFail) {
  tests.push_back({suite, name, function, shouldFail});
}

std::vector<std::string> Runner::suiteNames() const {
  std::vector<std::string> names;

  for (const auto& test : tests) {
    if (std::find(names.begin(), names.end(), test.suite) == names.end())
      names.push_back(test.suite);
  }

  return names;
}

int Runner::run(const std::string& filter) {
  auto [time, res] = Timer::run([this, filter] { return runTest(filter); });
  Printer::TestTime(time);

  return res;
}

int Runner::runTest(const std::string& filter) {
  int passed = 0;
  int failed = 0;
  int total = 0;

  auto selected = [&filter](const Test& test) {
    if (filter.empty())
      return true;

    return test.name == filter || test.suite == filter ||
           (test.suite + "." + test.name) == filter;
  };

  for (auto& test : tests) {
    if (!selected(test))
      continue;

    total++;

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

  if (total == 0) {
    std::cout << "No tests matched filter: " << filter << '\n';
    return 1;
  }

  Printer::OnTestProgramEnd(total, passed, failed);

  return failed;
}

} // namespace Test
