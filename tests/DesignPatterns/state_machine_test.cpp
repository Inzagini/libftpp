#include "DesignPatterns/StateMachine/state_machine.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <stdexcept>

enum class State { Idle, Walking, Running, Jumping };

// Needed because std::set/std::map need comparison
bool operator<(State a, State b) {
  return static_cast<int>(a) < static_cast<int>(b);
}

int main() {
  TestRunner runner;

  /*
      BASIC ACTION
  */

  runner.add("StateMachine", "ExecuteCurrentStateAction", []() {
    StateMachine<State> machine;

    bool called = false;

    machine.addState(State::Idle);

    machine.addAction(State::Idle, [&]() { called = true; });

    machine.update();

    assert(called);
  });

  /*
      BASIC TRANSITION
  */

  runner.add("StateMachine", "ExecuteTransition", []() {
    StateMachine<State> machine;

    bool transitioned = false;
    bool walking = false;

    machine.addState(State::Idle);
    machine.addState(State::Walking);

    machine.addTransition(State::Idle, State::Walking,
                          [&]() { transitioned = true; });

    machine.addAction(State::Walking, [&]() { walking = true; });

    machine.transitionTo(State::Walking);

    assert(transitioned);

    machine.update();

    assert(walking);
  });

  /*
      MULTIPLE STATES
  */

  runner.add("StateMachine", "MultipleTransitions", []() {
    StateMachine<State> machine;

    bool running = false;

    machine.addState(State::Idle);
    machine.addState(State::Walking);
    machine.addState(State::Running);

    machine.addTransition(State::Idle, State::Walking, []() {});

    machine.addTransition(State::Walking, State::Running, []() {});

    machine.addAction(State::Running, [&]() { running = true; });

    machine.transitionTo(State::Walking);

    machine.transitionTo(State::Running);

    machine.update();

    assert(running);
  });

  /*
      UPDATE WITHOUT ACTION
  */

  runner.add("StateMachine", "MissingActionThrows", []() {
    StateMachine<State> machine;

    machine.addState(State::Idle);

    bool thrown = false;

    try {
      machine.update();
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      TRANSITION DOES NOT EXIST
  */

  runner.add("StateMachine", "MissingTransitionThrows", []() {
    StateMachine<State> machine;

    machine.addState(State::Idle);
    machine.addState(State::Walking);

    bool thrown = false;

    try {
      machine.transitionTo(State::Walking);
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      UNKNOWN STATE
  */

  runner.add("StateMachine", "UnknownStateThrows", []() {
    StateMachine<State> machine;

    machine.addState(State::Idle);

    bool thrown = false;

    try {
      machine.transitionTo(State::Running);
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      ACTION ONLY RUNS FOR CURRENT STATE
  */

  runner.add("StateMachine", "OnlyCurrentStateRuns", []() {
    StateMachine<State> machine;

    bool idle = false;
    bool walking = false;

    machine.addState(State::Idle);
    machine.addState(State::Walking);

    machine.addAction(State::Idle, [&]() { idle = true; });

    machine.addAction(State::Walking, [&]() { walking = true; });

    machine.update();

    assert(idle);
    assert(!walking);
  });

  /*
      DUPLICATE STATE
  */

  runner.add("StateMachine", "DuplicateState", []() {
    StateMachine<State> machine;

    machine.addState(State::Idle);
    machine.addState(State::Idle);

    // should not crash
  });

  /*
      TRANSITION CALLBACK ONLY ON TRANSITION
  */

  runner.add("StateMachine", "TransitionCallbackNotRepeated", []() {
    StateMachine<State> machine;

    int count = 0;

    machine.addState(State::Idle);
    machine.addState(State::Walking);

    machine.addTransition(State::Idle, State::Walking, [&]() { count++; });

    machine.transitionTo(State::Walking);

    assert(count == 1);

    machine.update();

    assert(count == 1);
  });

  return runner.run();
}
