#pragma once

#include <functional>
#include <set>
template <typename TState> class StateMachine {

private:
  struct Transition {
    TState from;
    TState to;

    bool operator<(const Transition& other) const {
      if (from < other.from)
        return true;
      else if (other.from < from)
        return false;
      return to < other.to;
    }
  };

public:
  void addState(const TState& state);
  void addTransition(const TState& startState, const TState& finalState,
                     const std::function<void()>& f);
  void addAction(const TState& state, const std::function<void>& f);
  void transitionTo(const TState& state);
  void update();

private:
  std::set<TState> states;
  std::map<TState, std::function<void()>> actions;
  std::map<Transition, std::function<void()>> transitions;

  bool hasCurrentState = false;
  TState state;
};
