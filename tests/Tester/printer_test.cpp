#include "Tester/printer.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <chrono>

void register_Printer(Test::Runner& runner) {
  runner.add("Printer", "FormatTime", []() {
    using namespace std::chrono;

    assert(Test::Printer::formatTime(nanoseconds(9999)) == "9999 ns");
    assert(Test::Printer::formatTime(nanoseconds(10000)) == "10 µs");

    assert(Test::Printer::formatTime(microseconds(9999)) == "9999 µs");
    assert(Test::Printer::formatTime(microseconds(10000)) == "10 ms");

    assert(Test::Printer::formatTime(milliseconds(9999)) == "9999 ms");
    assert(Test::Printer::formatTime(milliseconds(10000)) == "10 s");

    assert(Test::Printer::formatTime(seconds(90)) == "90 s");
  });
}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Printer(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif
