#pragma once

#include <chrono>
#include <functional>
#include <utility>

struct Timer {
  template <typename T> static auto run(T&& f) {

    auto start = std::chrono::steady_clock::now();

    auto res = std::invoke(std::forward<T>(f));

    auto end = std::chrono::steady_clock::now();

    return std::pair{end - start, std::move(res)};
  }
};
