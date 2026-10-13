#ifndef LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_HPP
# define LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_HPP

#include <functional>
#include <unordered_map>
#include <vector>
template <typename T> class Observer {

private:
  std::unordered_map<T, std::vector<std::function<void()>>> events;

public:
  void subscribe(const T& event, const std::function<void()>& f);
  void notify(const T& event);
};

#include "observer.tpp"
#endif // LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_HPP
