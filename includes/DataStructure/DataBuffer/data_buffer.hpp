#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

class DataBuffer {

private:
  std::vector<char> _buffer;

  std::size_t _readpos{};
  std::size_t _writePos{};
  std::size_t _size{};

public:
  explicit DataBuffer(size_t capacity = 1024) { _buffer.resize(capacity); };

  void clear();
  std::size_t size() const;
  bool empty() const;
  bool full() const;

  template <typename T> DataBuffer& operator<<(const T& dataValue);
  template <typename T> DataBuffer& operator>>(T& dataValue);

  DataBuffer& operator<<(const std::string& dataValue);
  DataBuffer& operator>>(std::string& dataValue);

private:
  void writeBytes(const char* data, size_t size);
  void readBytes(char* data, size_t size);
};

#include "data_buffer.tpp"
