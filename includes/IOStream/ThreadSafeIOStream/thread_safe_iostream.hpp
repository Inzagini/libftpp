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

template <typename T>
ThreadSafeIOStream& ThreadSafeIOStream::operator<<(const T& data) {

  std::ostringstream stream;
  stream << data;

  std::string text = stream.str();

  std::lock_guard<std::mutex> lock(s_mutex);

  printPrefixIfNeeded();

  std::cout << text;

  updateLineState(text);

  return *this;
}

template <typename T>
ThreadSafeIOStream& ThreadSafeIOStream::operator>>(T& data) {
  std::lock_guard<std::mutex> lock(s_mutex);

  std::cin >> data;

  return *this;
}

template <typename T>
void ThreadSafeIOStream::prompt(const std::string& question, T& dest) {
  std::lock_guard<std::mutex> lock(s_mutex);

  printPrefixIfNeeded();

  std::cout << question;
  std::cin >> dest;

  m_startLine = true;
}
