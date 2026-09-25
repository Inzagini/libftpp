#pragma once

#include <atomic>
#include <sstream>
#include <string>
#include <utility>

class Log {

public:
  enum class Level { Debug, Info, Warning, Error };

  static void setLevel(Level level);
  static Level level();

  static void log(Level level, const std::string& message);

  template <typename... TArgs> static void debug(TArgs&&... args) {
    log(Level::Debug, concat(std::forward<TArgs>(args)...));
  }

  template <typename... TArgs> static void info(TArgs&&... args) {
    log(Level::Info, concat(std::forward<TArgs>(args)...));
  }

  template <typename... TArgs> static void warning(TArgs&&... args) {
    log(Level::Warning, concat(std::forward<TArgs>(args)...));
  }

  template <typename... TArgs> static void error(TArgs&&... args) {
    log(Level::Error, concat(std::forward<TArgs>(args)...));
  }

private:
  template <typename... TArgs> static std::string concat(TArgs&&... args) {
    std::ostringstream stream;
    (stream << ... << args);
    return stream.str();
  }

  static std::atomic<Level> _level;
};
