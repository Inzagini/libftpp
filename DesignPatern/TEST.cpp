#include "design_patterns.hpp"

#include <cassert>
#include <iostream>

/*
    MEMENTO TEST
*/

class Player : public Memento {
  friend class Memento;

private:
  int hp;
  int position;

  void _saveToSnapshot(Snapshot& snapshot) const override {
    snapshot["hp"] = hp;
    snapshot["position"] = position;
  }

  void _loadFromSnapshot(Snapshot& snapshot) override {
    hp = std::any_cast<int>(snapshot["hp"]);
    position = std::any_cast<int>(snapshot["position"]);
  }

public:
  Player(int h, int p) : hp(h), position(p) {}

  void damage(int amount) { hp -= amount; }

  void move(int amount) { position += amount; }

  int getHP() const { return hp; }

  int getPosition() const { return position; }
};

/*
    SINGLETON TEST
*/

class Logger : public Singleton<Logger> {
  friend class Singleton<Logger>;

private:
  int value;

  Logger(int v) : value(v) {}

public:
  int getValue() const { return value; }
};

/*
    OBSERVER TEST
*/

enum class Event { START, STOP };

int main() {

  std::cout << "===== DESIGN PATTERN TEST =====\n";

  /*
      MEMENTO
  */

  {
    std::cout << "\n[Memento]\n";

    Player player(100, 10);

    auto snapshot = player.save();

    player.damage(80);
    player.move(50);

    assert(player.getHP() == 20);
    assert(player.getPosition() == 60);

    player.load(snapshot);

    assert(player.getHP() == 100);
    assert(player.getPosition() == 10);

    std::cout << "Memento OK\n";
  }

  /*
      OBSERVER
  */

  {
    std::cout << "\n[Observer]\n";

    Observer<Event> observer;

    bool started = false;
    bool stopped = false;

    observer.subscribe(Event::START, [&]() { started = true; });

    observer.subscribe(Event::STOP, [&]() { stopped = true; });

    observer.notify(Event::START);
    observer.notify(Event::STOP);

    assert(started == true);
    assert(stopped == true);

    std::cout << "Observer OK\n";
  }

  /*
      SINGLETON
  */

  {
    std::cout << "\n[Singleton]\n";

    Singleton<Logger> manager;

    manager.instantiate(42);

    Logger* first = manager.instance();

    Logger* second = manager.instance();

    assert(first == second);

    assert(first->getValue() == 42);

    bool exceptionThrown = false;

    try {
      manager.instantiate(100);
    } catch (const std::exception&) {
      exceptionThrown = true;
    }

    assert(exceptionThrown == true);

    std::cout << "Singleton OK\n";
  }

  /*
      STATE MACHINE
  */

  {
    std::cout << "\n[StateMachine]\n";

    StateMachine<State> sm;

    sm.addState(State::Idle);
    sm.addState(State::Walking);
    sm.addState(State::Running);

    bool idleAction = false;
    bool walkingAction = false;
    bool runningAction = false;

    bool idleToWalking = false;
    bool walkingToRunning = false;

    sm.addAction(State::Idle, [&]() { idleAction = true; });

    sm.addAction(State::Walking, [&]() { walkingAction = true; });

    sm.addAction(State::Running, [&]() { runningAction = true; });

    sm.addTransition(State::Idle, State::Walking,
                     [&]() { idleToWalking = true; });

    sm.addTransition(State::Walking, State::Running,
                     [&]() { walkingToRunning = true; });

    // Initial state should be Idle
    sm.update();

    assert(idleAction);

    idleAction = false;

    // Idle -> Walking
    sm.transitionTo(State::Walking);

    assert(idleToWalking);

    sm.update();

    assert(walkingAction);

    // Walking -> Running
    sm.transitionTo(State::Running);

    assert(walkingToRunning);

    sm.update();

    assert(runningAction);

    // Invalid transition
    bool transitionException = false;

    try {
      sm.transitionTo(State::Idle);
    } catch (const std::exception&) {
      transitionException = true;
    }

    assert(transitionException);

    /*
        STATE MACHINE EDGE CASES
    */

    {
      std::cout << "\n[StateMachine Edge Cases]\n";

      /*
          Missing action
      */
      {
        StateMachine<State> sm;

        sm.addState(State::Idle);

        bool thrown = false;

        try {
          sm.update();
        } catch (const std::exception&) {
          thrown = true;
        }

        assert(thrown);

        std::cout << "Missing action exception OK\n";
      }

      /*
          Missing transition
      */
      {
        StateMachine<State> sm;

        sm.addState(State::Idle);
        sm.addState(State::Walking);

        bool thrown = false;

        try {
          sm.transitionTo(State::Walking);
        } catch (const std::exception&) {
          thrown = true;
        }

        assert(thrown);

        std::cout << "Missing transition exception OK\n";
      }

      /*
          Unknown state transition
      */
      {
        StateMachine<State> sm;

        sm.addState(State::Idle);

        bool thrown = false;

        try {
          sm.transitionTo(State::Running);
        } catch (const std::exception&) {
          thrown = true;
        }

        assert(thrown);

        std::cout << "Unknown state exception OK\n";
      }

      std::cout << "StateMachine Edge Cases OK\n";
    }

    std::cout << "StateMachine OK\n";
  }

  std::cout << "\nALL TESTS PASSED\n";

  return 0;
}
