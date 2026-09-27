#include "Network/Message/message.hpp"

Message::Message(Type type) : _type(type), _readPos(0) {}

Message::Type Message::type() const { return _type; }

const std::vector<uint8_t>& Message::data() const { return _data; }

std::vector<uint8_t>& Message::data() { return _data; }

void Message::resetRead() { _readPos = 0; }

void Message::serialize(const std::string& value) {
  std::uint32_t size = static_cast<uint32_t>(value.size());

  *this << size;

  const size_t oldSize = _data.size();
  _data.resize(oldSize + size);

  std::memcpy(_data.data() + oldSize, value.data(), size);
}

void Message::deserialize(std::string& value) {
  std::uint32_t size;
  deserialize(size);

  if (_readPos + size > _data.size())
    throw std::runtime_error("Read overflow");

  value.assign(reinterpret_cast<const char*>(_data.data() + _readPos), size);
  _readPos += size;
}
