#pragma once

#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

class ThreadSafeIOStream {

public:
  ThreadSafeIOStream() = default;
  ~ThreadSafeIOStream() = default;
  void setPrefix(const std::string& str);

  template <typename T> ThreadSafeIOStream& operator<<(const T& data);
  template <typename T> ThreadSafeIOStream& operator>>(T& data);
  template <typename T> void prompt(const std::string& question, T& dest);

private:
  void printPrefixIfNeeded();
  void updateLineState(const std::string& text);

private:
  std::string m_prefix;
  bool m_startLine = true;
  inline static std::mutex s_mutex;
};

extern thread_local ThreadSafeIOStream threadSafeCout;

#include "thread_safe_iostream.tpp"
