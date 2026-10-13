#ifndef LIBFTPP_INCLUDES_DESIGNPATTERNS_SINGLETON_SINGLETON_HPP
# define LIBFTPP_INCLUDES_DESIGNPATTERNS_SINGLETON_SINGLETON_HPP

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
#endif // LIBFTPP_INCLUDES_DESIGNPATTERNS_SINGLETON_SINGLETON_HPP
