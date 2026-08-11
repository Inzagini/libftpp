#include "Mathematics/Random2DCoordinateGenerator/random_2D_coordinate_generator.hpp"
#include <cstdint>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(const long long seed)
    : _seed(seed) {}

long long Random2DCoordinateGenerator::seed() const { return _seed; }

long long Random2DCoordinateGenerator::operator()(const long long& x,
                                                  const long long& y) const {

  std::uint64_t h = static_cast<std::uint64_t>(_seed);

  h ^= static_cast<std::uint64_t>(x) + c + (h << 6) + (h >> 2);
  h ^= static_cast<std::uint64_t>(y) + c + (h << 6) + (h >> 2);

  h ^= h >> 30;
  h *= splitA;
  h ^= h >> 27;
  h *= splitB;
  h ^= h >> 31;

  return h;
}
