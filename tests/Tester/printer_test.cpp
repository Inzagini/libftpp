#include "Tester/printer.hpp"

#include <cassert>
#include <chrono>

int main() {
  using namespace std::chrono;

  assert(Test::Printer::formatTime(nanoseconds(9999)) == "9999 ns");
  assert(Test::Printer::formatTime(nanoseconds(10000)) == "10 us");

  assert(Test::Printer::formatTime(microseconds(9999)) == "9999 us");
  assert(Test::Printer::formatTime(microseconds(10000)) == "10 ms");

  assert(Test::Printer::formatTime(milliseconds(9999)) == "9999 ms");
  assert(Test::Printer::formatTime(milliseconds(10000)) == "10 s");

  assert(Test::Printer::formatTime(seconds(90)) == "90 s");

  return 0;
}
