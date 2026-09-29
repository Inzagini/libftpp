#pragma once

#include <memory>
#include <stdexcept>
#include <utility>
template <typename Ttype> class Singleton {

private:
  static std::unique_ptr<Ttype> _instance;

public:
  Ttype* instance();
  template <typename... Targs> void instantiate(Targs&&... args);
  void destroy();
};

#include "singleton.tpp"
