#ifndef LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_TPP
# define LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_TPP

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
#endif // LIBFTPP_INCLUDES_DESIGNPATTERNS_OBSERVER_OBSERVER_TPP
