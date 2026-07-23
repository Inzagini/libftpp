#include "DataStructure/DataBuffer/data_buffer.hpp"
#include <stdexcept>

std::size_t DataBuffer::size() const { return _buffer.size(); }

void DataBuffer::clear() {
  _readpos = 0;
  _writePos = 0;
  _size = 0;
  _buffer.clear();
}

void DataBuffer::writeBytes(const char* data, size_t count) {
  if (count > _buffer.size() - _size)
    throw std::runtime_error("Buffer full");

  for (size_t i{}; i < count; i++) {
    _buffer[_writePos] = data[i];

    _writePos++;
    if (_writePos == _buffer.size())
      _writePos = 0;
  }

  _size += count;
}

void DataBuffer::readBytes(char* output, size_t count) {
  if (count > _size)
    throw std::runtime_error("Not enough data");

  for (size_t i{}; i < count; i++) {
    output[i] = _buffer[_readpos];

    _readpos++;
    if (_readpos == _buffer.size())
      _readpos = 0;
  }
  _size -= count;
}

bool DataBuffer::empty() const { return _buffer.size() == 0; }

bool DataBuffer::full() const { return _buffer.size() == _size; }
