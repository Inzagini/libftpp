#pragma once

#include <cstddef>
#include <new>
#include <stack>
#include <stdexcept>
#include <vector>

template <typename T> class Pool {

  size_t _size;

  struct Slot {
    bool used;
    void* mem;
  };

  void* _memBuffer;
  std::vector<Slot> _memSlot;
  std::stack<Slot*> _memStack;

  class Object {
    friend class Pool<T>;
    Slot* _slot;
    Pool* _owner;

    Object(Pool* owner, Slot* slot);

  public:
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    Object(Object&& other) noexcept;
    Object& operator=(Object&& other) noexcept;

    ~Object();

    T* operator->();
    const T* operator->() const;
  };

public:
  explicit Pool(const size_t size = 2);

  Pool(const Pool&) = delete;
  Pool& operator=(const Pool&) = delete;

  ~Pool();

  void resize(const size_t& numberOfObjectStored);

  template <typename... TArgs> Object acquire(TArgs&&... p_args);

private:
  void release(Slot* slot);
};

#include "pool.tpp"
