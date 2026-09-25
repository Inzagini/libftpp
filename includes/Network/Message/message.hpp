#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

class Message {

public:
  using Type = int;

  explicit Message(Type type);
  Type type() const;
  const std::vector<uint8_t>& data() const;
  std::vector<uint8_t>& data();
  void resetRead();

  template <typename T> Message& operator<<(const T& value);

  template <typename T> Message& operator>>(T& value);

private:
  template <typename T> void serialize(const T& value);
  void serialize(const std::string& value);

  template <typename T> void deserialize(T& value);
  void deserialize(std::string& value);

private:
  Type _type;
  std::vector<uint8_t> _data;
  size_t _readPos;
};
