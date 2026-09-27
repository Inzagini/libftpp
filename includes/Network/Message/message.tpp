#pragma once

#include <cstring>
#include <stdexcept>
#include <type_traits>

template <typename T> Message& Message::operator<<(const T& value) {
  serialize(value);
  return *this;
}

template <typename T> Message& Message::operator>>(T& value) {
  deserialize(value);
  return *this;
}

template <typename T> void Message::serialize(const T& value) {
  static_assert(std::is_trivially_copyable_v<T>, "Value not copy able");

  const size_t oldSize = _data.size();

  _data.resize(oldSize + sizeof(T));

  std::memcpy(_data.data() + oldSize, &value, sizeof(T));
}

template <typename T> void Message::deserialize(T& value) {
  static_assert(std::is_trivially_copyable_v<T>, "Value not copy able");

  if (_readPos + sizeof(T) > _data.size())
    throw std::runtime_error("Read overflow");

  std::memcpy(&value, _data.data() + _readPos, sizeof(T));
  _readPos += sizeof(T);
}
