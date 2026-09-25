#pragma once

#include "DataStructure/RingBuffer/ring_buffer.hpp"
#include <cassert>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T>
RingBuffer<T>::RingBuffer(std::size_t capacity) : _capacity(capacity) {
  if (_capacity == 0)
    throw std::invalid_argument("RingBuffer capacity must be > 0");

  _buffer = allocate(_capacity);
}

template <typename T> RingBuffer<T>::~RingBuffer() {
  clear();

  if (_buffer)
    deallocate(_buffer);
}

template <typename T>
RingBuffer<T>::RingBuffer(const RingBuffer& other)
    : _capacity(other._capacity) {
  if (_capacity == 0)
    return;

  _buffer = allocate(_capacity);

  try {
    for (; _size < other._size; ++_size)
      new (_buffer + _size) T(other._buffer[other.physical(_size)]);
  } catch (...) {
    for (std::size_t i{}; i < _size; ++i)
      _buffer[i].~T();
    deallocate(_buffer);
    throw;
  }

  _tail = _size;
}

template <typename T>
RingBuffer<T>::RingBuffer(RingBuffer&& other) noexcept
    : _buffer(other._buffer), _head(other._head), _tail(other._tail),
      _size(other._size), _capacity(other._capacity) {
  other._buffer = nullptr;
  other._head = other._tail = other._size = other._capacity = 0;
}

template <typename T>
RingBuffer<T>& RingBuffer<T>::operator=(const RingBuffer& other) {
  if (this == &other)
    return *this;

  RingBuffer copy(other);
  swap(copy);

  return *this;
}

template <typename T>
RingBuffer<T>& RingBuffer<T>::operator=(RingBuffer&& other) noexcept {
  if (this == &other)
    return *this;

  clear();

  if (_buffer)
    deallocate(_buffer);

  _buffer = other._buffer;
  _head = other._head;
  _tail = other._tail;
  _size = other._size;
  _capacity = other._capacity;

  other._buffer = nullptr;
  other._head = other._tail = other._size = other._capacity = 0;

  return *this;
}

template <typename T> void RingBuffer<T>::swap(RingBuffer& other) noexcept {
  std::swap(_buffer, other._buffer);
  std::swap(_head, other._head);
  std::swap(_tail, other._tail);
  std::swap(_size, other._size);
  std::swap(_capacity, other._capacity);
}

template <typename T> bool RingBuffer<T>::push(const T& value) {
  if (full())
    return false;

  new (_buffer + _tail) T(value);
  advanceWrite();

  return true;
}

template <typename T> bool RingBuffer<T>::push(T&& value) {
  if (full())
    return false;

  new (_buffer + _tail) T(std::move(value));
  advanceWrite();

  return true;
}

template <typename T>
template <typename... TArgs>
bool RingBuffer<T>::emplace(TArgs&&... args) {
  if (full())
    return false;

  new (_buffer + _tail) T(std::forward<TArgs>(args)...);
  advanceWrite();

  return true;
}

template <typename T> bool RingBuffer<T>::pushOverwrite(const T& value) {
  if (!full())
    return push(value);

  _buffer[_tail].~T();
  new (_buffer + _tail) T(value);

  _tail = (_tail + 1) % _capacity;
  _head = _tail;

  return true;
}

template <typename T> T RingBuffer<T>::pop() {
  if (empty())
    throw std::runtime_error("RingBuffer is empty");

  T value = std::move(_buffer[_head]);
  _buffer[_head].~T();

  _head = (_head + 1) % _capacity;
  --_size;

  return value;
}

template <typename T> T& RingBuffer<T>::front() {
  assert(!empty());
  return _buffer[_head];
}

template <typename T> const T& RingBuffer<T>::front() const {
  assert(!empty());
  return _buffer[_head];
}

template <typename T> T& RingBuffer<T>::back() {
  assert(!empty());
  return _buffer[physical(_size - 1)];
}

template <typename T> const T& RingBuffer<T>::back() const {
  assert(!empty());
  return _buffer[physical(_size - 1)];
}

template <typename T> T& RingBuffer<T>::operator[](std::size_t index) {
  assert(index < _size);
  return _buffer[physical(index)];
}

template <typename T>
const T& RingBuffer<T>::operator[](std::size_t index) const {
  assert(index < _size);
  return _buffer[physical(index)];
}

template <typename T> std::size_t RingBuffer<T>::size() const { return _size; }

template <typename T> std::size_t RingBuffer<T>::capacity() const {
  return _capacity;
}

template <typename T> bool RingBuffer<T>::empty() const { return _size == 0; }

template <typename T> bool RingBuffer<T>::full() const {
  return _size == _capacity;
}

template <typename T> void RingBuffer<T>::clear() {
  for (std::size_t i{}; i < _size; ++i)
    _buffer[physical(i)].~T();

  _head = _tail = _size = 0;
}

template <typename T> T* RingBuffer<T>::allocate(std::size_t capacity) {
  return static_cast<T*>(
      ::operator new(capacity * sizeof(T), std::align_val_t(alignof(T))));
}

template <typename T> void RingBuffer<T>::deallocate(T* buffer) {
  ::operator delete(buffer, std::align_val_t(alignof(T)));
}

template <typename T>
std::size_t RingBuffer<T>::physical(std::size_t logical) const {
  return (_head + logical) % _capacity;
}

template <typename T> void RingBuffer<T>::advanceWrite() {
  _tail = (_tail + 1) % _capacity;
  ++_size;
}
