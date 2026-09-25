#pragma once

#include <cassert>
#include <cstddef>
#include <new>
#include <stdexcept>
#include <utility>

template <typename T> class RingBuffer {

public:
  explicit RingBuffer(std::size_t capacity) : _capacity(capacity) {
    if (_capacity == 0)
      throw std::invalid_argument("RingBuffer capacity must be > 0");

    _buffer = allocate(_capacity);
  }

  ~RingBuffer() {
    clear();

    if (_buffer)
      deallocate(_buffer);
  }

  RingBuffer(const RingBuffer& other) : _capacity(other._capacity) {
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

  RingBuffer(RingBuffer&& other) noexcept
      : _buffer(other._buffer), _head(other._head), _tail(other._tail),
        _size(other._size), _capacity(other._capacity) {
    other._buffer = nullptr;
    other._head = other._tail = other._size = other._capacity = 0;
  }

  RingBuffer& operator=(const RingBuffer& other) {
    if (this == &other)
      return *this;

    RingBuffer copy(other);
    swap(copy);

    return *this;
  }

  RingBuffer& operator=(RingBuffer&& other) noexcept {
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

  void swap(RingBuffer& other) noexcept {
    std::swap(_buffer, other._buffer);
    std::swap(_head, other._head);
    std::swap(_tail, other._tail);
    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
  }

  bool push(const T& value) {
    if (full())
      return false;

    new (_buffer + _tail) T(value);
    advanceWrite();

    return true;
  }

  bool push(T&& value) {
    if (full())
      return false;

    new (_buffer + _tail) T(std::move(value));
    advanceWrite();

    return true;
  }

  template <typename... TArgs> bool emplace(TArgs&&... args) {
    if (full())
      return false;

    new (_buffer + _tail) T(std::forward<TArgs>(args)...);
    advanceWrite();

    return true;
  }

  // Overwrites the oldest element when full instead of rejecting.
  bool pushOverwrite(const T& value) {
    if (!full())
      return push(value);

    _buffer[_tail].~T();
    new (_buffer + _tail) T(value);

    _tail = (_tail + 1) % _capacity;
    _head = _tail;

    return true;
  }

  T pop() {
    if (empty())
      throw std::runtime_error("RingBuffer is empty");

    T value = std::move(_buffer[_head]);
    _buffer[_head].~T();

    _head = (_head + 1) % _capacity;
    --_size;

    return value;
  }

  T& front() {
    assert(!empty());
    return _buffer[_head];
  }

  const T& front() const {
    assert(!empty());
    return _buffer[_head];
  }

  T& back() {
    assert(!empty());
    return _buffer[physical(_size - 1)];
  }

  const T& back() const {
    assert(!empty());
    return _buffer[physical(_size - 1)];
  }

  T& operator[](std::size_t index) {
    assert(index < _size);
    return _buffer[physical(index)];
  }

  const T& operator[](std::size_t index) const {
    assert(index < _size);
    return _buffer[physical(index)];
  }

  std::size_t size() const { return _size; }

  std::size_t capacity() const { return _capacity; }

  bool empty() const { return _size == 0; }

  bool full() const { return _size == _capacity; }

  void clear() {
    for (std::size_t i{}; i < _size; ++i)
      _buffer[physical(i)].~T();

    _head = _tail = _size = 0;
  }

private:
  static T* allocate(std::size_t capacity) {
    return static_cast<T*>(
        ::operator new(capacity * sizeof(T), std::align_val_t(alignof(T))));
  }

  static void deallocate(T* buffer) {
    ::operator delete(buffer, std::align_val_t(alignof(T)));
  }

  std::size_t physical(std::size_t logical) const {
    return (_head + logical) % _capacity;
  }

  void advanceWrite() {
    _tail = (_tail + 1) % _capacity;
    ++_size;
  }

private:
  T* _buffer = nullptr;

  std::size_t _head = 0;
  std::size_t _tail = 0;
  std::size_t _size = 0;
  std::size_t _capacity = 0;
};
