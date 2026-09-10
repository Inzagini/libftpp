#include "Mathematics/PerlinNoise2D/perlin_noise_2D.hpp"
#include "Tester/runner.hpp"

#include <cmath>
#include <stdexcept>

int main() {
  Test::Runner runner;

  // 1. Same coordinates must always produce the same value.
  runner.add("PerlinNoise2D", "Deterministic", []() {
    PerlinNoise2D noise;

    const float a = noise(2.3f, 4.7f);
    const float b = noise(2.3f, 4.7f);

    if (a != b)
      throw std::runtime_error("Same coordinates produced different values");
  });

  // 2. Different coordinates should normally produce different values.
  runner.add("PerlinNoise2D", "DifferentCoordinates", []() {
    PerlinNoise2D noise;

    const float a = noise(2.3f, 4.7f);
    const float b = noise(2.4f, 4.7f);

    if (a == b)
      throw std::runtime_error("Different coordinates produced the same value");
  });

  // 3. Integer coordinates should be deterministic as well.
  runner.add("PerlinNoise2D", "IntegerCoordinates", []() {
    PerlinNoise2D noise;

    const float a = noise(10.0f, 20.0f);
    const float b = noise(10.0f, 20.0f);

    if (a != b)
      throw std::runtime_error("Integer coordinates are not deterministic");
  });

  // 4. Nearby points should generally have similar values.
  runner.add("PerlinNoise2D", "Smoothness", []() {
    PerlinNoise2D noise;

    const float a = noise(2.0f, 3.0f);
    const float b = noise(2.01f, 3.0f);

    const float difference = std::fabs(a - b);

    if (difference > 0.1f)
      throw std::runtime_error("Noise changes too much between nearby points");
  });

  // 5. Moving continuously through a cell should produce finite values.
  runner.add("PerlinNoise2D", "FiniteValues", []() {
    PerlinNoise2D noise;

    for (float x = -10.0f; x <= 10.0f; x += 0.1f) {
      for (float y = -10.0f; y <= 10.0f; y += 0.1f) {

        const float value = noise(x, y);

        if (!std::isfinite(value))
          throw std::runtime_error("Noise produced NaN or infinity");
      }
    }
  });

  // 6. Test negative coordinates.
  runner.add("PerlinNoise2D", "NegativeCoordinates", []() {
    PerlinNoise2D noise;

    const float a = noise(-2.3f, -4.7f);
    const float b = noise(-2.3f, -4.7f);

    if (!std::isfinite(a))
      throw std::runtime_error("Negative coordinates produced invalid value");

    if (a != b)
      throw std::runtime_error("Negative coordinates are not deterministic");
  });

  // 7. Test crossing an integer grid boundary.
  runner.add("PerlinNoise2D", "GridBoundary", []() {
    PerlinNoise2D noise;

    const float before = noise(1.999f, 2.5f);
    const float after = noise(2.001f, 2.5f);

    const float difference = std::fabs(before - after);

    if (difference > 0.1f)
      throw std::runtime_error("Noise is not smooth across grid boundary");
  });

  // 8. Repeated sampling should always give the same result.
  runner.add("PerlinNoise2D", "RepeatedSampling", []() {
    PerlinNoise2D noise;

    const float expected = noise(123.456f, 789.123f);

    for (int i = 0; i < 100; ++i) {
      if (noise(123.456f, 789.123f) != expected)
        throw std::runtime_error("Repeated sampling produced different values");
    }
  });

  return runner.run();
}
