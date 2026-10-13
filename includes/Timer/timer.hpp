#ifndef LIBFTPP_INCLUDES_TIMER_TIMER_HPP
# define LIBFTPP_INCLUDES_TIMER_TIMER_HPP

#include <chrono>

class Timer {
public:
  using Clock = std::chrono::steady_clock;
  using Duration = Clock::duration;

  Timer();
  explicit Timer(const Duration& duration);

  void setDuration(const Duration& duration);
  void reset();

  bool timedOut() const;
  bool expired() const;

  Duration elapsed() const;
  Duration remaining() const;

private:
  Duration _duration;
  Clock::time_point _start;
};
#endif // LIBFTPP_INCLUDES_TIMER_TIMER_HPP
