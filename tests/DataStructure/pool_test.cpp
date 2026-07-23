#include "DataStructure/Pool/pool.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <string>

class Player {
public:
  static int alive;

private:
  int hp;
  std::string name;

public:
  Player(int h, std::string n) : hp(h), name(std::move(n)) { alive++; }

  ~Player() { alive--; }

  int getHP() const { return hp; }

  const std::string& getName() const { return name; }
};

int Player::alive = 0;

int main() {
  TestRunner runner;

  /*
      BASIC ACQUIRE
  */

  runner.add("Pool", "AcquireObject", []() {
    Pool<Player> pool(2);

    auto player = pool.acquire(100, "Knight");

    assert(player->getHP() == 100);
    assert(player->getName() == "Knight");
  });

  /*
      CONSTRUCTOR ARGUMENTS
  */

  runner.add("Pool", "ForwardConstructorArguments", []() {
    Pool<Player> pool(1);

    auto player = pool.acquire(50, "Mage");

    assert(player->getHP() == 50);
    assert(player->getName() == "Mage");
  });

  /*
      FULL POOL
  */

  runner.add("Pool", "AcquireWhenFullThrows", []() {
    Pool<Player> pool(1);

    auto first = pool.acquire(10, "A");

    bool thrown = false;

    try {
      auto second = pool.acquire(20, "B");
    } catch (const std::logic_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      AUTOMATIC RELEASE
  */

  runner.add("Pool", "ObjectAutomaticallyReleased", []() {
    Pool<Player> pool(1);

    {
      auto player = pool.acquire(100, "Temp");
    }

    auto second = pool.acquire(200, "Reuse");

    assert(second->getHP() == 200);
  });

  /*
      DESTRUCTOR CALLED
  */

  runner.add("Pool", "DestructorCalled", []() {
    assert(Player::alive == 0);

    {
      Pool<Player> pool(2);

      auto a = pool.acquire(10, "A");

      auto b = pool.acquire(20, "B");

      assert(Player::alive == 2);
    }

    assert(Player::alive == 0);
  });

  /*
      MOVE CONSTRUCTOR
  */

  runner.add("Pool", "MoveObject", []() {
    Pool<Player> pool(1);

    auto first = pool.acquire(100, "Original");

    auto second = std::move(first);

    assert(second->getHP() == 100);
  });

  /*
      MOVE ASSIGNMENT
  */

  runner.add("Pool", "MoveAssignment", []() {
    Pool<Player> pool(2);

    auto first = pool.acquire(100, "First");

    auto second = pool.acquire(200, "Second");

    second = std::move(first);

    assert(second->getHP() == 100);
  });

  /*
      RESIZE EMPTY POOL
  */

  runner.add("Pool", "ResizeEmptyPool", []() {
    Pool<Player> pool(2);

    pool.resize(5);

    auto object = pool.acquire(100, "AfterResize");

    assert(object->getHP() == 100);
  });

  /*
      RESIZE WITH ACTIVE OBJECTS
  */

  runner.add("Pool", "ResizeWithObjectsThrows", []() {
    Pool<Player> pool(2);

    auto object = pool.acquire(100, "Active");

    bool thrown = false;

    try {
      pool.resize(5);
    } catch (const std::logic_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      REUSE AFTER RELEASE
  */

  runner.add("Pool", "ReuseReleasedMemory", []() {
    Pool<Player> pool(1);

    {
      auto object = pool.acquire(10, "Old");
    }

    auto object = pool.acquire(999, "New");

    assert(object->getHP() == 999);
  });

  return runner.run();
}
