#pragma once

template <typename... TArgs> std::string Log::concat(TArgs&&... args) {
  std::ostringstream stream;
  (stream << ... << args);
  return stream.str();
}

template <typename... TArgs> void Log::debug(TArgs&&... args) {
  log(Level::Debug, concat(std::forward<TArgs>(args)...));
}

template <typename... TArgs> void Log::info(TArgs&&... args) {
  log(Level::Info, concat(std::forward<TArgs>(args)...));
}

template <typename... TArgs> void Log::warning(TArgs&&... args) {
  log(Level::Warning, concat(std::forward<TArgs>(args)...));
}

template <typename... TArgs> void Log::error(TArgs&&... args) {
  log(Level::Error, concat(std::forward<TArgs>(args)...));
}
