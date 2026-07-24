#include "IOStream/ThreadSafeIOStream/thread_safe_iostream.hpp"

void ThreadSafeIOStream::setPrefix(const std::string& prefix) {
  m_prefix = prefix;
}

void ThreadSafeIOStream::printPrefixIfNeeded() {
  if (m_startLine) {
    std::cout << m_prefix;
    m_startLine = false;
  }
}

void ThreadSafeIOStream::updateLineState(const std::string& text) {
  if (!text.empty() && text.back() == '\n')
    m_startLine = true;
}
