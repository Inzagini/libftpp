#include "IOStream/Log/log.hpp"

std::atomic<Log::Level> Log::_level{Log::Level::Info};

namespace {
std::string_view levelToString(Log::Level level) {
  switch (level) {
  case Log::Level::Debug:
    return "DEBUG";
  case Log::Level::Info:
    return "INFO";
  case Log::Level::Warning:
    return "WARNING";
  case Log::Level::Error:
    return "ERROR";
  }
  return "UNKNOWN";
}
} // namespace

void Log::setLevel(Level level) { _level.store(level); }

Log::Level Log::level() { return _level.load(); }

void Log::log(Level level, const std::string& message) {
  if (level < _level.load())
    return;

  threadSafeCout << "[" << levelToString(level) << "] " << message << "\n";
}
