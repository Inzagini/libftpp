#pragma once

#include <memory>
#include <stdexcept>
template <typename Ttype> class Singleton {

private:
  static std::unique_ptr<Ttype> _instance;

public:
  Ttype* instance() { return _instance; }

  template <typename... Targs> void instantiate(Targs&&... args) {
    if (_instance != nullptr)
      throw std::runtime_error("Instance is already present");

    _instance = std::make_unique<Ttype>(std::forward<Targs>(args)...);
  }

  void destroy() { _instance.reset(); }
};

template <typename T> std::unique_ptr<T> Singleton<T>::_instance = nullptr;
