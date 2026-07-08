#pragma once

#include <cstddef>
#include <new>
#include <stack>
#include <stdexcept>

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

    Object(Pool* owner, Slot* slot) : _owner(owner), _slot(slot) {};

  public:
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;

    Object(Object&& other) noexcept : _slot(other._slot), _owner(other._owner) {

      other._slot = nullptr;
      other._owner = nullptr;
    }
    Object& operator=(Object&& other) noexcept {
      if (this != &other) {

        if (_owner && _slot)
          _owner->release(_slot);

        this->_slot = other._slot;
        this->_owner = other._owner;

        other._slot = nullptr;
        other._owner = nullptr;
      }

      return *this;
    }

    ~Object() {
      if (_owner && _slot)
        _owner->release(_slot);
    };

    T* operator->() { return static_cast<T*>(_slot->mem); };
    const T* operator->() const { return static_cast<const T*>(_slot->mem); };
  };

public:
  explicit Pool(const size_t size = 2) : _size(size) {

    _memBuffer =
        ::operator new(_size * sizeof(T), std::align_val_t(alignof(T)));
    T* start = static_cast<T*>(_memBuffer);
    _memSlot.reserve(_size);

    for (size_t i{}; i < _size; i++) {
      _memSlot.push_back(Slot{.used = false, .mem = start + i});
      _memStack.push(&_memSlot.back());
    }
  }

  Pool(const Pool&) = delete;
  Pool& operator=(const Pool&) = delete;

  ~Pool() {
    for (Slot& slot : _memSlot) {
      if (slot.used)
        static_cast<T*>(slot.mem)->~T();
    }
    ::operator delete(_memBuffer, std::align_val_t(alignof(T)));
  }

  void resize(const size_t& numberOfObjectStored) {

    if (_memStack.size() != _size)
      throw std::logic_error(
          "Cannot resize while object are still allocated\n");

    void* newMem = ::operator new(sizeof(T) * numberOfObjectStored,
                                  std::align_val_t(alignof(T)));

    while (!_memStack.empty())
      _memStack.pop();
    _memSlot.clear();

    _size = numberOfObjectStored;
    _memSlot.reserve(_size);

    ::operator delete(_memBuffer, std::align_val_t(alignof(T)));
    _memBuffer = newMem;
    T* start = static_cast<T*>(_memBuffer);

    for (size_t i{}; i < _size; i++) {
      _memSlot.push_back(Slot{.used = false, .mem = start + i});
      _memStack.push(&_memSlot.back());
    }
  }

  template <typename... TArgs> Object acquire(TArgs&&... p_args) {
    if (_memStack.empty())
      throw std::logic_error("Pool has no more empty memory slot\n");

    Slot* slot = _memStack.top();
    _memStack.pop();
    slot->used = true;
    new (slot->mem) T(std::forward<TArgs>(p_args)...);

    return Object(this, slot);
  }

private:
  void release(Slot* slot) {

    if (!slot)
      return;

    static_cast<T*>(slot->mem)->~T();
    slot->used = false;
    _memStack.push(slot);
  }
};
