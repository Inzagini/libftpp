#pragma once

#include <functional>
#include <map>
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
                     const std::function<void()>& lambda);
  void addAction(const TState& state, const std::function<void()>& lambda);
  void transitionTo(const TState& state);
  void update();

private:
  std::set<TState> states;
  std::map<TState, std::function<void()>> actions;
  std::map<Transition, std::function<void()>> transitions;

  bool hasCurrentState = false;
  TState state;
};

template <typename TState>
void StateMachine<TState>::addState(const TState& newState) {
  states.insert(newState);

  if (!hasCurrentState) {
    state = newState;
    hasCurrentState = true;
  }
}

template <typename TState>
void StateMachine<TState>::addAction(const TState& state,
                                     const std::function<void()>& lambda) {
  actions[state] = lambda;
}

template <typename TState>
void StateMachine<TState>::addTransition(const TState& startState,
                                         const TState& finalState,
                                         const std::function<void()>& lambda) {
  transitions[{startState, finalState}] = lambda;
}

template <typename TState>
void StateMachine<TState>::transitionTo(const TState& newState) {
  if (!states.contains(newState))
    throw std::runtime_error("State does not exist");

  Transition transition{state, newState};

  auto it = transitions.find(transition);

  if (it == transitions.end())
    throw std::runtime_error("Transition does not exist");

  it->second();

  state = newState;
}

template <typename TState> void StateMachine<TState>::update() {
  auto it = actions.find(state);

  if (it == actions.end())
    throw std::runtime_error("No action for state");

  it->second();
}
