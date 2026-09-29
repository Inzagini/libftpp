#pragma once

template <typename T> std::unique_ptr<T> Singleton<T>::_instance = nullptr;

template <typename Ttype> Ttype* Singleton<Ttype>::instance() {
  return _instance.get();
}

template <typename Ttype>
template <typename... Targs>
void Singleton<Ttype>::instantiate(Targs&&... args) {
  if (_instance != nullptr)
    throw std::runtime_error("Instance is already present");

  _instance.reset(new Ttype(std::forward<Targs>(args)...));
}

template <typename Ttype> void Singleton<Ttype>::destroy() {
  _instance.reset();
}
