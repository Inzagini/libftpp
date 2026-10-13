#ifndef LIBFTPP_INCLUDES_TESTER_RUNNER_HPP
# define LIBFTPP_INCLUDES_TESTER_RUNNER_HPP

#include "Tester/printer.hpp"
#include "Chronometer/chronometer.hpp"

#include <functional>
#include <iostream>
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

  int run(const std::string& filter = "");
  int runTest(const std::string& filter = "");
  std::vector<std::string> suiteNames() const;
};

} // namespace Test
#endif // LIBFTPP_INCLUDES_TESTER_RUNNER_HPP
