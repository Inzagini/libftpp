#pragma once

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
