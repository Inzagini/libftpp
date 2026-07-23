#include "DataStructure/DataBuffer/data_buffer.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <string>

struct TestStruct {
  int id;
  double value;
};

int main() {
  TestRunner runner;

  /*
      BASIC INT WRITE / READ
  */

  runner.add("DataBuffer", "WriteReadInt", []() {
    DataBuffer buffer(16);

    int input = 42;

    int output = 0;

    buffer << input;

    buffer >> output;

    assert(output == 42);
  });

  /*
      MULTIPLE VALUES ORDER
  */

  runner.add("DataBuffer", "PreserveOrder", []() {
    DataBuffer buffer(32);

    int a = 10;
    double b = 3.14;

    buffer << a;
    buffer << b;

    int readA;
    double readB;

    buffer >> readA;
    buffer >> readB;

    assert(readA == a);
    assert(readB == b);
  });

  /*
      STRING SERIALIZATION
  */

  runner.add("DataBuffer", "WriteReadString", []() {
    DataBuffer buffer(64);

    std::string input = "Hello DataBuffer";

    std::string output;

    buffer << input;

    buffer >> output;

    assert(output == input);
  });

  /*
      EMPTY BUFFER
  */

  runner.add("DataBuffer", "EmptyInitially", []() {
    DataBuffer buffer(32);

    assert(buffer.empty());
  });

  /*
      SIZE TRACKING
  */

  runner.add("DataBuffer", "SizeChanges", []() {
    DataBuffer buffer(32);

    int value = 100;

    buffer << value;

    assert(buffer.size() == 4);

    int result;

    buffer >> result;

    assert(buffer.empty());
  });

  /*
      BUFFER FULL
  */

  runner.add("DataBuffer", "WriteWhenFullThrows", []() {
    DataBuffer buffer(4);

    int value = 100;

    buffer << value;

    assert(buffer.full());

    bool thrown = false;

    try {
      buffer << value;
    } catch (const std::runtime_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      READ WITHOUT DATA
  */

  runner.add("DataBuffer", "ReadEmptyThrows", []() {
    DataBuffer buffer(16);

    int value;

    bool thrown = false;

    try {
      buffer >> value;
    } catch (const std::runtime_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      WRAP AROUND RING BUFFER
  */

  runner.add("DataBuffer", "RingBufferWrapAround", []() {
    DataBuffer buffer(8);

    int a = 10;
    int b = 20;

    buffer << a;

    buffer >> a;

    buffer << b;

    int result;

    buffer >> result;

    assert(result == b);
  });

  /*
      CUSTOM TRIVIAL STRUCT
  */

  runner.add("DataBuffer", "StructSerialization", []() {
    DataBuffer buffer(32);

    TestStruct input{5, 7.5};

    TestStruct output{};

    buffer << input;

    buffer >> output;

    assert(output.id == 5);
    assert(output.value == 7.5);
  });

  /*
      CLEAR BUFFER
  */

  runner.add("DataBuffer", "ClearBuffer", []() {
    DataBuffer buffer(32);

    int value = 10;

    buffer << value;

    buffer.clear();

    assert(buffer.empty());
  });

  /*
      STRING EMPTY
  */

  runner.add("DataBuffer", "EmptyString", []() {
    DataBuffer buffer(32);

    std::string input;

    std::string output;

    buffer << input;

    buffer >> output;

    assert(output.empty());
  });

  return runner.run();
}
