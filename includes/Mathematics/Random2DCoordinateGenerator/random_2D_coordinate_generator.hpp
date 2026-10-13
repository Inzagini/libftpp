#ifndef LIBFTPP_INCLUDES_MATHEMATICS_RANDOM2DCOORDINATEGENERATOR_RANDOM_2D_COORDINATE_GENERATOR_HPP
# define LIBFTPP_INCLUDES_MATHEMATICS_RANDOM2DCOORDINATEGENERATOR_RANDOM_2D_COORDINATE_GENERATOR_HPP

#include <cstdint>

// Using splitmix64 algorithm
class Random2DCoordinateGenerator {

private:
  long long _seed;
  const std::uint64_t C = 0x9e3779b97f4a7c15ULL;
  const std::uint64_t splitA = 0xbf58476d1ce4e5b9ULL;
  const std::uint64_t splitB = 0x94d049bb133111ebULL;

public:
  explicit Random2DCoordinateGenerator(const long long seed);
  long long seed() const;
  long long operator()(const long long& x, const long long& y) const;
};
#endif // LIBFTPP_INCLUDES_MATHEMATICS_RANDOM2DCOORDINATEGENERATOR_RANDOM_2D_COORDINATE_GENERATOR_HPP
