#pragma once

#include <cstddef>

template <typename T> class RingBuffer {

public:
  explicit RingBuffer(std::size_t capacity);
  ~RingBuffer();

  RingBuffer(const RingBuffer& other);
  RingBuffer(RingBuffer&& other) noexcept;
  RingBuffer& operator=(const RingBuffer& other);
  RingBuffer& operator=(RingBuffer&& other) noexcept;

  void swap(RingBuffer& other) noexcept;

  bool push(const T& value);
  bool push(T&& value);
  template <typename... TArgs> bool emplace(TArgs&&... args);
  bool pushOverwrite(const T& value);

  T pop();

  T& front();
  const T& front() const;
  T& back();
  const T& back() const;
  T& operator[](std::size_t index);
  const T& operator[](std::size_t index) const;

  std::size_t size() const;
  std::size_t capacity() const;
  bool empty() const;
  bool full() const;
  void clear();

private:
  static T* allocate(std::size_t capacity);
  static void deallocate(T* buffer);

  std::size_t physical(std::size_t logical) const;
  void advanceWrite();

  T* _buffer = nullptr;

  std::size_t _head{};
  std::size_t _tail{};
  std::size_t _size{};
  std::size_t _capacity{};
};

#include "ring_buffer.tpp"
