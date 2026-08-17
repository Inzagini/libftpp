#include "Network/Message/message.hpp"

Message::Message(Type type) : _type(type) {}

Message::Type Message::type() const { return _type; }

const std::vector<uint8_t>& Message::data() const { return _data; }

std::vector<uint8_t>& Message::data() { return _data; }

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

template <typename T> void Message::serialize(const std::string& value) {
  std::uint32_t size = static_cast<uint32_t>(value.size());

  serialize(size);

  const size_t oldSize = _data.size();
  _data.resize(oldSize + size);

  std::memcpy(_data.data() + oldSize, value.data(), size);
}

template <typename T> void Message::deserialize(const T& value) {
  static_assert(std::is_trivially_copyable_v<T>, "Value not copy able");

  if (_readPos + +sizeof(T) > _data.size())
    throw std::runtime_error("Read overflow");

  std::memcpy(&value, _data.data(), sizeof(T));
  _readPos += sizeof(T);
}

template <typename T> void Message::deserialize(const std::string& value) {

  std::uint32_t size;
  deserialize(size);

  if (_readPos + +sizeof(T) > _data.size())
    throw std::runtime_error("Read overflow");

  value.assign(reinterpret_cast<const char*>(_data.data() + _readPos), size);
  _readPos += size;
}
