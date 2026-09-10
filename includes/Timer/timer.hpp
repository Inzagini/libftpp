#pragma once

#include <chrono>
#include <functional>

struct Timer {

  static std::chrono::nanoseconds run(const std::function<void()>& f) {

    auto start = std::chrono::steady_clock::now();

    f();

    auto end = std::chrono::steady_clock::now();

    return end - start;
  }
};
