#pragma once

#include <cmath>
#include <cstdint>

class PerlinNoise2D {

private:
  static std::uint32_t hash(int x, int y);
  static float gradient(int x, int y, float dx, float dy);
  static float fade(float t);
  static float lerp(float a, float b, float t);

public:
  PerlinNoise2D() = default;
  float operator()(float x, float y) const;
  float sample(float x, float y) const;
};
