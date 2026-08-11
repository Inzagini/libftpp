#include "../utils/test_runner.hpp"
#include "Mathematics/IVector3/ivector3.hpp"

#include <cmath>
#include <stdexcept>

int main() {
  TestRunner runner;

  runner.add("IVector3", "Addition", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{4, 5, 6};

    auto result = a + b;

    if (result != IVector3<int>{5, 7, 9})
      throw std::runtime_error("Addition failed");
  });

  runner.add("IVector3", "Subtraction", []() {
    IVector3<int> a{5, 7, 9};
    IVector3<int> b{1, 2, 3};

    auto result = a - b;

    if (result != IVector3<int>{4, 5, 6})
      throw std::runtime_error("Subtraction failed");
  });

  runner.add("IVector3", "ScalarMultiplyRight", []() {
    IVector3<int> a{1, 2, 3};

    auto result = a * 5;

    if (result != IVector3<int>{5, 10, 15})
      throw std::runtime_error("Right scalar multiplication failed");
  });

  runner.add("IVector3", "ScalarMultiplyLeft", []() {
    IVector3<int> a{1, 2, 3};

    auto result = 5 * a;

    if (result != IVector3<int>{5, 10, 15})
      throw std::runtime_error("Left scalar multiplication failed");
  });

  runner.add("IVector3", "VectorDivision", []() {
    IVector3<int> a{10, 20, 30};
    IVector3<int> b{10, 20, 30};

    auto result = a / b;

    if (result != IVector3<int>{1, 1, 1})
      throw std::runtime_error("Scalar division failed");
  });

  runner.add("IVector3", "ScalarDivisionRight", []() {
    IVector3<int> a{10, 20, 30};

    auto result = a / 10;

    if (result != IVector3<int>{1, 2, 3})
      throw std::runtime_error("Scalar division failed");
  });

  runner.add("IVector3", "ScalarDivisionLeft", []() {
    IVector3<int> a{10, 20, 25};

    auto result = 100 / a;

    if (result != IVector3<int>{10, 5, 4})
      throw std::runtime_error("Scalar division failed");
  });

  runner.add("IVector3", "Equality", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{1, 2, 3};

    if (!(a == b))
      throw std::runtime_error("Equality failed");
  });

  runner.add("IVector3", "Inequality", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{1, 2, 4};

    if (!(a != b))
      throw std::runtime_error("Inequality failed");
  });

  runner.add("IVector3", "Length", []() {
    IVector3<int> a{2, 3, 6};

    // sqrt(4 + 9 + 36) = 7
    if (std::abs(a.length() - 7.0f) > 0.0001f)
      throw std::runtime_error("Length failed");
  });

  runner.add("IVector3", "Normalize", []() {
    IVector3<int> a{2, 3, 6};

    auto result = a.normalize();

    if (std::abs(result.x - 2.0f / 7.0f) > 0.0001f)
      throw std::runtime_error("Normalize x failed");

    if (std::abs(result.y - 3.0f / 7.0f) > 0.0001f)
      throw std::runtime_error("Normalize y failed");

    if (std::abs(result.z - 6.0f / 7.0f) > 0.0001f)
      throw std::runtime_error("Normalize z failed");
  });

  runner.add("IVector3", "NormalizeZeroVector", []() {
    IVector3<int> a{0, 0, 0};

    auto result = a.normalize();

    if (result != IVector3<float>{0.0f, 0.0f, 0.0f})
      throw std::runtime_error("Zero vector normalization failed");
  });

  runner.add("IVector3", "DotProduct", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{4, 5, 6};

    // 1*4 + 2*5 + 3*6 = 32
    if (a.dot(b) != 32)
      throw std::runtime_error("Dot product failed");
  });

  runner.add("IVector3", "CrossProduct", []() {
    IVector3<int> a{1, 0, 0};
    IVector3<int> b{0, 1, 0};

    auto result = a.cross(b);

    if (result != IVector3<int>{0, 0, 1})
      throw std::runtime_error("Cross product failed");
  });

  runner.add("IVector3", "CrossProductGeneral", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{4, 5, 6};

    auto result = a.cross(b);

    // (-3, 6, -3)
    if (result != IVector3<int>{-3, 6, -3})
      throw std::runtime_error("General cross product failed");
  });

  runner.add("IVector3", "CrossProductParallel", []() {
    IVector3<int> a{1, 2, 3};
    IVector3<int> b{2, 4, 6};

    auto result = a.cross(b);

    if (result != IVector3<int>{0, 0, 0})
      throw std::runtime_error("Parallel cross product failed");
  });

  runner.add("IVector3", "CrossProductPerpendicular", []() {
    IVector3<int> x{1, 0, 0};
    IVector3<int> y{0, 1, 0};

    auto result = x.cross(y);

    if (result != IVector3<int>{0, 0, 1})
      throw std::runtime_error("Perpendicular cross product failed");
  });

  return runner.run();
}
