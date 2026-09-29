#pragma once

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
