#pragma once

#include <functional>
#include <unordered_map>
#include <vector>
template <typename T> class Observer {

private:
  std::unordered_map<T, std::vector<std::function<void()>>> events;

public:
  void subscribe(const T& event, const std::function<void()>& f) {
    events[event].push_back(f);
  }

  void notify(const T& event) {
    auto it = events.find(event);
    if (it == events.end())
      return;

    for (auto& callback : it->second) {
      callback();
    }
  }
};
