#include "Timer/timer.hpp"

Timer::Timer() : _duration(Duration::zero()), _start(Clock::now()) {}

Timer::Timer(const Duration& duration)
    : _duration(duration), _start(Clock::now()) {}

void Timer::setDuration(const Duration& duration) {
  _duration = duration;
  _start = Clock::now();
}

void Timer::reset() { _start = Clock::now(); }

bool Timer::timedOut() const { return elapsed() >= _duration; }

bool Timer::expired() const { return timedOut(); }

Timer::Duration Timer::elapsed() const { return Clock::now() - _start; }

Timer::Duration Timer::remaining() const {
  const Duration left = _duration - elapsed();
  return left > Duration::zero() ? left : Duration::zero();
}
