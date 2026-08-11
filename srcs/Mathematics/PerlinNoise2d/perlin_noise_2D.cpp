#include "Mathematics/PerlinNoise2D/perlin_noise_2D.hpp"

std::uint32_t PerlinNoise2D::hash(int x, int y) {
  const std::uint32_t C1 = 374761393u;
  const std::uint32_t C2 = 668265263u;
  const std::uint32_t C3 = 1274126177u;

  std::uint32_t h = static_cast<std::uint32_t>(x) * C1;
  h += static_cast<std::uint32_t>(y) * C2;
  h = (h ^ (h >> 13)) * C3;
  h ^= h >> 16;

  return h;
}

float PerlinNoise2D::gradient(int x, int y, float dx, float dy) {

  const std::uint32_t h = hash(x, y);

  switch (h & 3u) {
  case 0:
    return dx + dy;

  case 1:
    return -dx + dy;

  case 2:
    return dx - dy;

  default:
    return -dx - dy;
  }
}

float PerlinNoise2D::fade(float t) {
  return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f);
}

float PerlinNoise2D::lerp(float a, float b, float t) { return a + t * (b - a); }

float PerlinNoise2D::operator()(float x, float y) const {

  const int x0 = static_cast<int>(std::floor(x));
  const int y0 = static_cast<int>(std::floor(y));
  const int x1 = x0 + 1;
  const int y1 = y0 + 1;

  const float xf = x - static_cast<float>(x0);
  const float yf = y - static_cast<float>(y0);

  const float n00 = gradient(x0, y0, xf, yf);
  const float n10 = gradient(x1, y0, xf - 1.0f, yf);
  const float n01 = gradient(x0, y1, xf, yf - 1.0f);
  const float n11 = gradient(x1, y1, xf - 1.0f, yf - 1.0f);

  const float u = fade(xf);
  const float v = fade(yf);

  const float nx0 = lerp(n00, n10, u);
  const float nx1 = lerp(n01, n11, u);

  return lerp(nx0, nx1, v);
}
