#ifndef LIBFTPP_INCLUDES_TIMER_TIMER_HPP
# define LIBFTPP_INCLUDES_TIMER_TIMER_HPP

#include <chrono>
#include <functional>
#include <utility>

struct Timer {
  template <typename T> static auto run(T&& f);
};

#include "timer.tpp"
#endif // LIBFTPP_INCLUDES_TIMER_TIMER_HPP
