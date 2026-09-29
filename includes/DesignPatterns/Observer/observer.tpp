#pragma once

template <typename T>
void Observer<T>::subscribe(const T& event, const std::function<void()>& f) {
  events[event].push_back(f);
}

template <typename T> void Observer<T>::notify(const T& event) {
  auto it = events.find(event);
  if (it == events.end())
    return;

  for (auto& callback : it->second) {
    callback();
  }
}
