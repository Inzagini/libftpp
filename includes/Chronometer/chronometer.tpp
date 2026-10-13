#ifndef LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_TPP
# define LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_TPP

template <typename T> auto Chronometer::run(T&& f) {
  auto start = std::chrono::steady_clock::now();
  auto res = std::invoke(std::forward<T>(f));
  auto end = std::chrono::steady_clock::now();
  return std::pair{end - start, std::move(res)};
}
#endif // LIBFTPP_INCLUDES_CHRONOMETER_CHRONOMETER_TPP
