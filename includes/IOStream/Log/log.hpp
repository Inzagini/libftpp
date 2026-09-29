#pragma once

#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"
#include <atomic>
#include <sstream>
#include <string>
#include <string_view>
#include <utility>

class Log {

public:
  enum class Level { Debug, Info, Warning, Error };

  static void setLevel(Level level);
  static Level level();

  static void log(Level level, const std::string& message);

  template <typename... TArgs> static void debug(TArgs&&... args);
  template <typename... TArgs> static void info(TArgs&&... args);
  template <typename... TArgs> static void warning(TArgs&&... args);
  template <typename... TArgs> static void error(TArgs&&... args);

private:
  template <typename... TArgs> static std::string concat(TArgs&&... args);

  static std::atomic<Level> _level;
};

#include "log.tpp"
