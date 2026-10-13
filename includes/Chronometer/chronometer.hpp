#ifndef LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_HPP
# define LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_HPP

#include <chrono>
#include <functional>
#include <utility>

struct Chronometer {
  template <typename T> static auto run(T&& f);
};

#include "chronometer.tpp"
#endif // LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_HPP
