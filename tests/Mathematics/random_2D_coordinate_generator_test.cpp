#include "../utils/test_runner.hpp"
#include "Mathematics/Random2DCoordinateGenerator/random_2D_coordinate_generator.hpp"

#include <stdexcept>

int main() {
  TestRunner runner;

  runner.add("Random2DCoordinateGenerator", "Seed", []() {
    Random2DCoordinateGenerator generator(42);

    if (generator.seed() != 42)
      throw std::runtime_error("Seed was not stored correctly");
  });

  // ------------------------------------------------------------
  // Determinism
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "SameInputSameResult", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(10, 20);
    const long long second = generator(10, 20);

    if (first != second)
      throw std::runtime_error(
          "Same seed and coordinates produced different results");
  });

  runner.add("Random2DCoordinateGenerator", "SameInputRepeated", []() {
    Random2DCoordinateGenerator generator(42);

    const long long expected = generator(100, 200);

    for (int i = 0; i < 100; ++i) {
      if (generator(100, 200) != expected)
        throw std::runtime_error("Result changed between identical calls");
    }
  });

  // ------------------------------------------------------------
  // Different coordinates
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "DifferentX", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(10, 20);
    const long long second = generator(11, 20);

    if (first == second)
      throw std::runtime_error("Changing X produced the same result");
  });

  runner.add("Random2DCoordinateGenerator", "DifferentY", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(10, 20);
    const long long second = generator(10, 21);

    if (first == second)
      throw std::runtime_error("Changing Y produced the same result");
  });

  runner.add("Random2DCoordinateGenerator", "DifferentCoordinates", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(10, 20);
    const long long second = generator(100, 200);

    if (first == second)
      throw std::runtime_error(
          "Different coordinates produced the same result");
  });

  // ------------------------------------------------------------
  // Different seeds
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "DifferentSeeds", []() {
    Random2DCoordinateGenerator firstGenerator(42);
    Random2DCoordinateGenerator secondGenerator(43);

    const long long first = firstGenerator(10, 20);
    const long long second = secondGenerator(10, 20);

    if (first == second)
      throw std::runtime_error("Different seeds produced the same result");
  });

  // ------------------------------------------------------------
  // Negative coordinates
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "NegativeCoordinates", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(-10, -20);
    const long long second = generator(-10, -20);

    if (first != second)
      throw std::runtime_error("Negative coordinates are not deterministic");
  });

  runner.add("Random2DCoordinateGenerator", "MixedCoordinates", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(-10, 20);
    const long long second = generator(10, -20);

    if (first == second)
      throw std::runtime_error(
          "Mixed positive/negative coordinates produced same result");
  });

  // ------------------------------------------------------------
  // Boundary values
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "ZeroCoordinates", []() {
    Random2DCoordinateGenerator generator(42);

    const long long first = generator(0, 0);
    const long long second = generator(0, 0);

    if (first != second)
      throw std::runtime_error("Zero coordinates are not deterministic");
  });

  runner.add("Random2DCoordinateGenerator", "LargeCoordinates", []() {
    Random2DCoordinateGenerator generator(42);

    const long long x = 9223372036854770000LL;
    const long long y = -9223372036854770000LL;

    const long long first = generator(x, y);
    const long long second = generator(x, y);

    if (first != second)
      throw std::runtime_error("Large coordinates are not deterministic");
  });

  // ------------------------------------------------------------
  // Different generator instances
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "DifferentInstancesSameSeed", []() {
    Random2DCoordinateGenerator first(42);
    Random2DCoordinateGenerator second(42);

    if (first(10, 20) != second(10, 20))
      throw std::runtime_error("Same seed does not produce same result "
                               "across instances");
  });

  // ------------------------------------------------------------
  // Order independence
  // ------------------------------------------------------------

  runner.add("Random2DCoordinateGenerator", "OrderIndependent", []() {
    Random2DCoordinateGenerator generator(42);

    const long long firstA = generator(10, 20);
    const long long firstB = generator(30, 40);

    const long long secondA = generator(10, 20);
    const long long secondB = generator(30, 40);

    if (firstA != secondA || firstB != secondB)
      throw std::runtime_error("Result depends on previous calls");
  });

  return runner.run();
}
