#pragma once

#include <chrono>
#include <functional>
#include <utility>

struct Timer {
  template <typename T> static auto run(T&& f);
};

#include "timer.tpp"
