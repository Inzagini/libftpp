#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>

extern thread_local ThreadSafeIOStream threadSafeCout;

class CoutCapture {
public:
  void start() { old = std::cout.rdbuf(buffer.rdbuf()); }

  std::string stop() {
    std::cout.rdbuf(old);
    return buffer.str();
  }

private:
  std::stringstream buffer;
  std::streambuf* old;
};

int main() {
  Test::Runner runner;

  /*
      BASIC OUTPUT
  */

  runner.add("ThreadSafeIOStream", "BasicOutput", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("");

    threadSafeCout << "Hello";

    std::string output = capture.stop();

    assert(output == "Hello");
  });

  /*
      PREFIX PRINTED
  */

  runner.add("ThreadSafeIOStream", "PrefixOutput", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[Worker] ");

    threadSafeCout << "Hello";

    std::string output = capture.stop();

    assert(output == "[Worker] Hello");
  });

  /*
      PREFIX ONLY ONCE PER LINE
  */

  runner.add("ThreadSafeIOStream", "PrefixOnlyOnce", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[A] ");

    threadSafeCout << "Hello ";
    threadSafeCout << "World";

    std::string output = capture.stop();

    assert(output == "[A] Hello World");
  });

  /*
      PREFIX AFTER NEWLINE
  */

  runner.add("ThreadSafeIOStream", "PrefixAfterNewLine", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[A] ");

    threadSafeCout << "First\n";
    threadSafeCout << "Second";

    std::string output = capture.stop();

    assert(output == "[A] First\n"
                     "[A] Second");
  });

  /*
      EMPTY STRING OUTPUT
  */

  runner.add("ThreadSafeIOStream", "EmptyString", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[A] ");

    threadSafeCout << "";

    std::string output = capture.stop();

    assert(output == "[A] ");
  });

  /*
      MULTIPLE DATA TYPES
  */

  runner.add("ThreadSafeIOStream", "DifferentTypes", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("");

    threadSafeCout << 42;
    threadSafeCout << " ";
    threadSafeCout << 3.14;

    std::string output = capture.stop();

    assert(output == "42 3.14");
  });

  /*
      MULTIPLE LINES
  */

  runner.add("ThreadSafeIOStream", "MultipleLines", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[T] ");

    threadSafeCout << "A\n";
    threadSafeCout << "B\n";
    threadSafeCout << "C";

    std::string output = capture.stop();

    assert(output == "[T] A\n"
                     "[T] B\n"
                     "[T] C");
  });

  /*
      THREAD LOCAL PREFIX
  */

  runner.add("ThreadSafeIOStream", "ThreadLocalPrefix", []() {
    CoutCapture capture;

    capture.start();

    std::thread t1([]() {
      threadSafeCout.setPrefix("[A] ");

      threadSafeCout << "Hello\n";
    });

    std::thread t2([]() {
      threadSafeCout.setPrefix("[B] ");

      threadSafeCout << "World\n";
    });

    t1.join();
    t2.join();

    std::string output = capture.stop();

    bool correct = output == "[A] Hello\n[B] World\n" ||
                   output == "[B] World\n[A] Hello\n";

    assert(correct);
  });

  /*
      PREFIX CHANGE
  */

  runner.add("ThreadSafeIOStream", "ChangePrefix", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[OLD] ");

    threadSafeCout << "A\n";

    threadSafeCout.setPrefix("[NEW] ");

    threadSafeCout << "B";

    std::string output = capture.stop();

    assert(output == "[OLD] A\n"
                     "[NEW] B");
  });

  /*
      NEWLINE ONLY
  */

  runner.add("ThreadSafeIOStream", "OnlyNewLine", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[A] ");

    threadSafeCout << "\n";

    std::string output = capture.stop();

    assert(output == "[A] \n");
  });

  /*
      LONG OUTPUT
  */

  runner.add("ThreadSafeIOStream", "LongOutput", []() {
    CoutCapture capture;

    capture.start();

    threadSafeCout.setPrefix("[A] ");

    std::string text(10000, 'x');

    threadSafeCout << text;

    std::string output = capture.stop();

    assert(output.size() == text.size() + 4);
    assert(output.starts_with("[A] "));
  });

  return runner.run();
}
