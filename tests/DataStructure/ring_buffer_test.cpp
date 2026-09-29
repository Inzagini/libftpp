#include "DataStructure/RingBuffer/ring_buffer.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <stdexcept>
#include <string>
#include <utility>

void register_RingBuffer(Test::Runner& runner) {

  runner.add("RingBuffer", "PushPopFifo", []() {
    RingBuffer<int> buffer(4);

    assert(buffer.push(1));
    assert(buffer.push(2));
    assert(buffer.push(3));

    assert(buffer.size() == 3);
    assert(buffer.pop() == 1);
    assert(buffer.pop() == 2);
    assert(buffer.pop() == 3);
    assert(buffer.empty());
  });

  runner.add("RingBuffer", "Wraparound", []() {
    RingBuffer<int> buffer(4);

    for (int i = 0; i < 4; ++i)
      assert(buffer.push(i));

    assert(buffer.pop() == 0);
    assert(buffer.pop() == 1);

    assert(buffer.push(4));
    assert(buffer.push(5));
    assert(buffer.full());

    for (int i = 2; i < 6; ++i)
      assert(buffer.pop() == i);

    assert(buffer.empty());
  });

  runner.add("RingBuffer", "PushWhenFullReturnsFalse", []() {
    RingBuffer<int> buffer(2);

    assert(buffer.push(1));
    assert(buffer.push(2));
    assert(!buffer.push(3));
    assert(buffer.size() == 2);
    assert(buffer.full());
    assert(buffer.pop() == 1);
  });

  runner.add("RingBuffer", "PopEmptyThrows", []() {
    RingBuffer<int> buffer(2);

    bool thrown = false;

    try {
      buffer.pop();
    } catch (const std::runtime_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  runner.add("RingBuffer", "FrontAndBack", []() {
    RingBuffer<int> buffer(4);

    buffer.push(10);
    buffer.push(20);
    buffer.push(30);

    assert(buffer.front() == 10);
    assert(buffer.back() == 30);

    buffer.pop();
    assert(buffer.front() == 20);

    buffer.push(40);
    assert(buffer.back() == 40);
  });

  runner.add("RingBuffer", "IndexAfterWraparound", []() {
    RingBuffer<int> buffer(4);

    for (int i = 0; i < 4; ++i)
      buffer.push(i);

    buffer.pop();
    buffer.pop();
    buffer.push(4);
    buffer.push(5);

    assert(buffer.size() == 4);

    for (std::size_t i = 0; i < buffer.size(); ++i)
      assert(buffer[i] == static_cast<int>(i) + 2);
  });

  runner.add("RingBuffer", "ClearFreesElements", []() {
    RingBuffer<std::string> buffer(4);

    buffer.push("a");
    buffer.push("b");
    buffer.clear();

    assert(buffer.empty());
    assert(buffer.capacity() == 4);

    assert(buffer.push("c"));
    assert(buffer.pop() == "c");
  });

  runner.add("RingBuffer", "CopiesLogicalOrder", []() {
    RingBuffer<int> original(4);

    for (int i = 1; i <= 4; ++i)
      original.push(i);

    original.pop();
    original.pop();
    original.push(5);
    original.push(6);

    RingBuffer<int> copy = original;

    assert(copy.size() == original.size());

    for (std::size_t i = 0; i < copy.size(); ++i)
      assert(copy[i] == original[i]);

    assert(copy.pop() == 3);
  });

  runner.add("RingBuffer", "MoveLeavesSourceReusable", []() {
    RingBuffer<std::string> original(4);

    original.push("x");
    original.push("y");

    RingBuffer<std::string> moved = std::move(original);

    assert(moved.size() == 2);
    assert(moved.pop() == "x");
    assert(original.empty());
  });

  runner.add("RingBuffer", "CopyAssignKeepsSource", []() {
    RingBuffer<int> a(3);
    a.push(1);
    a.push(2);

    RingBuffer<int> b(1);
    b = a;

    assert(b.size() == 2);
    assert(b[0] == 1);
    assert(b[1] == 2);
    assert(a.size() == 2);
  });

  runner.add("RingBuffer", "PushOverwriteKeepsNewest", []() {
    RingBuffer<int> buffer(3);

    buffer.push(1);
    buffer.push(2);
    buffer.push(3);
    buffer.pushOverwrite(4);

    assert(buffer.size() == 3);
    assert(buffer.front() == 2);
    assert(buffer.back() == 4);
    assert(buffer.pop() == 2);
    assert(buffer.pop() == 3);
    assert(buffer.pop() == 4);
  });

  runner.add("RingBuffer", "ZeroCapacityThrows", []() {
    bool thrown = false;

    try {
      RingBuffer<int> buffer(0);
    } catch (const std::invalid_argument&) {
      thrown = true;
    }

    assert(thrown);
  });

}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_RingBuffer(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif

