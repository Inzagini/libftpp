#pragma once

#include <cstring>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

class DataBuffer {

private:
  std::vector<char> _buffer;

  std::size_t _readpos = 0;
  std::size_t _writePos = 0;
  std::size_t _size = 0;

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

template <typename T> DataBuffer& DataBuffer::operator<<(const T& dataValue) {

  static_assert(std::is_trivially_copyable_v<T>,
                "Type must by trivially copyable");

  writeBytes(reinterpret_cast<const char*>(&dataValue), sizeof(T));

  return *this;
}

template <typename T> DataBuffer& DataBuffer::operator>>(T& dataValue) {

  static_assert(std::is_trivially_copyable_v<T>,
                "Type must by trivially copyable");

  readBytes(reinterpret_cast<char*>(&dataValue), sizeof(T));

  return *this;
}

DataBuffer& DataBuffer::operator<<(const std::string& dataValue) {

  std::size_t lenght = dataValue.size();

  *this << lenght;

  writeBytes(dataValue.data(), lenght);

  return *this;
}

DataBuffer& DataBuffer::operator>>(std::string& dataValue) {

  std::size_t lenght;
  *this >> lenght;

  dataValue.resize(lenght);

  readBytes(dataValue.data(), lenght);

  return *this;
}
