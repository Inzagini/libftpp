#pragma once

#include <cstddef>
#include <stack>
#include <stdexcept>

template <typename T> class Pool {

  size_t _size;
  void *_memBuffer;
  std::stack<T *> _memStack;

  class Object {

    T *_obj;
    Pool *_owner;

  public:
    Object(const Object &) = delete;
    Object &operator=(const Object &) = delete;

    Object(Pool *owner, T *obj) : _owner(owner), _obj(obj) {};
    Object(Object &&other) noexcept : _obj(other._obj), _owner(other._owner) {

      other._obj = nullptr;
      other._owner = nullptr;
    }
    Object &operator=(Object &&other) noexcept {
      if (this != &other) {

        if (_owner && _obj)
          _owner->release(_obj);

        this->_obj = other._obj;
        this->_owner = other._owner;

        other._obj = nullptr;
        other._owner = nullptr;
      }

      return *this;
    }

    ~Object() {
      if (_owner && _obj)
        _owner->release(_obj);
    };

    T *operator->() { return _obj; };
    const T *operator->() const { return _obj; };
  };

  Pool(const size_t size = 2) : _size(size) {

    _memBuffer = ::operator new(_size * sizeof(T));

    T *start = static_cast<T *>(_memBuffer);

    for (size_t i{}; i < size; i++)
      _memStack.push(start + i);
  }

public:
  Pool(const Pool &) = delete;
  Pool &operator=(const Pool &) = delete;

  ~Pool() { ::operator delete(_memBuffer); }

  void resize(const size_t &numberOfObjectStored) {

    if (_memStack.size() != _size)
      throw std::logic_error(
          "Cannot resize while object are still allocated\n");

    void *newMem = ::operator new(sizeof(T) * numberOfObjectStored);

    while (!_memStack.empty())
      _memStack.pop();

    ::operator delete(_memBuffer);
    _memBuffer = newMem;
    T *start = static_cast<T *>(_memBuffer);

    for (size_t i{}; i < numberOfObjectStored; i++)
      _memStack.push(start + i);

    _size = numberOfObjectStored;
  }

  template <typename... TArgs> Object acquire(TArgs &&...p_args) {
    if (_memStack.empty())
      throw std::logic_error("Pool has no more empty memory slot\n");

    T *slot = _memStack.top();
    _memStack.pop();

    auto obj = new (slot) T(std::forward<TArgs>(p_args)...);
    return Object(this, obj);
  }

private:
  void release(T *pointer) {
    pointer->~T();
    _memStack.push(pointer);
  }
};
