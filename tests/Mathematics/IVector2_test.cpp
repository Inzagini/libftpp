// #include "../utils/test_runner.hpp"
#include "Mathematics/IVector2/ivector2.hpp"
#include "Tester/runner.hpp"

#include <cmath>
#include <stdexcept>

int main() {
  Test::Runner runner;

  runner.add("IVector2", "Addition", []() {
    IVector2<int> a{1, 2};
    IVector2<int> b{3, 4};

    auto result = a + b;

    if (result.x != 4 || result.y != 6)
      throw std::runtime_error("Addition failed");
  });

  runner.add("IVector2", "Subtraction", []() {
    IVector2<int> a{5, 7};
    IVector2<int> b{2, 3};

    auto result = a - b;

    if (result.x != 3 || result.y != 4)
      throw std::runtime_error("Subtraction failed");
  });

  runner.add("IVector2", "ScalarMultiplyRight", []() {
    IVector2<int> a{2, 3};

    auto result = a * 5;

    if (result.x != 10 || result.y != 15)
      throw std::runtime_error("Right scalar multiplication failed");
  });

  runner.add("IVector2", "ScalarMultiplyLeft", []() {
    IVector2<int> a{2, 3};

    auto result = 5 * a;

    if (result.x != 10 || result.y != 15)
      throw std::runtime_error("Left scalar multiplication failed");
  });

  runner.add("IVector2", "Length", []() {
    IVector2<int> a{3, 4};

    if (std::abs(a.length() - 5.0f) > 0.0001f)
      throw std::runtime_error("Length failed");
  });

  runner.add("IVector2", "Normalize", []() {
    IVector2<int> a{3, 4};

    auto n = a.normalize();

    if (std::abs(n.x - 0.6f) > 0.001f)
      throw std::runtime_error("Normalize x failed");

    if (std::abs(n.y - 0.8f) > 0.001f)
      throw std::runtime_error("Normalize y failed");
  });

  runner.add("IVector2", "DotProduct", []() {
    IVector2<int> a{2, 3};
    IVector2<int> b{4, 5};

    if (a.dot(b) != 23)
      throw std::runtime_error("Dot product failed");
  });

  runner.add("IVector2", "CrossProduct", []() {
    IVector2<int> a{2, 3};
    IVector2<int> b{4, 5};

    if (a.cross(b) != -2)
      throw std::runtime_error("Cross product failed");
  });

  runner.add("IVector2", "Equality", []() {
    IVector2<int> a{1, 2};
    IVector2<int> b{1, 2};

    if (!(a == b))
      throw std::runtime_error("Equality failed");
  });

  runner.add("IVector2", "Inequality", []() {
    IVector2<int> a{1, 2};
    IVector2<int> b{2, 1};

    if (!(a != b))
      throw std::runtime_error("Inequality failed");
  });

  return runner.run();
}
