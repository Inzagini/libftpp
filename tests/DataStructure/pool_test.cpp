#include "DataStructure/Pool/pool.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <string>

class PoolPlayer {
public:
  static int alive;

private:
  int hp;
  std::string name;

public:
  PoolPlayer(int h, std::string n) : hp(h), name(std::move(n)) { alive++; }

  ~PoolPlayer() { alive--; }

  int getHP() const { return hp; }

  const std::string& getName() const { return name; }
};

int PoolPlayer::alive = 0;

void register_Pool(Test::Runner& runner) {

  // BASIC ACQUIRE
  runner.add("Pool", "AcquireObject", []() {
    Pool<PoolPlayer> pool(2);

    auto player = pool.acquire(100, "Knight");

    assert(player->getHP() == 100);
    assert(player->getName() == "Knight");
  });

  // CONSTRUCTOR ARGUMENTS
  runner.add("Pool", "ForwardConstructorArguments", []() {
    Pool<PoolPlayer> pool(1);

    auto player = pool.acquire(50, "Mage");

    assert(player->getHP() == 50);
    assert(player->getName() == "Mage");
  });

  // FULL POOL
  runner.add("Pool", "AcquireWhenFullThrows", []() {
    Pool<PoolPlayer> pool(1);

    auto first = pool.acquire(10, "A");

    bool thrown = false;

    try {
      auto second = pool.acquire(20, "B");
    } catch (const std::logic_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  // AUTOMATIC RELEASE
  runner.add("Pool", "ObjectAutomaticallyReleased", []() {
    Pool<PoolPlayer> pool(1);

    {
      auto player = pool.acquire(100, "Temp");
    }

    auto second = pool.acquire(200, "Reuse");

    assert(second->getHP() == 200);
  });

  // DESTRUCTOR CALLED
  runner.add("Pool", "DestructorCalled", []() {
    assert(PoolPlayer::alive == 0);

    {
      Pool<PoolPlayer> pool(2);

      auto a = pool.acquire(10, "A");

      auto b = pool.acquire(20, "B");

      assert(PoolPlayer::alive == 2);
    }

    assert(PoolPlayer::alive == 0);
  });

  // MOVE CONSTRUCTOR
  runner.add("Pool", "MoveObject", []() {
    Pool<PoolPlayer> pool(1);

    auto first = pool.acquire(100, "Original");

    auto second = std::move(first);

    assert(second->getHP() == 100);
  });

  // MOVE ASSIGNMENT
  runner.add("Pool", "MoveAssignment", []() {
    Pool<PoolPlayer> pool(2);

    auto first = pool.acquire(100, "First");

    auto second = pool.acquire(200, "Second");

    second = std::move(first);

    assert(second->getHP() == 100);
  });

  // RESIZE EMPTY POOL
  runner.add("Pool", "ResizeEmptyPool", []() {
    Pool<PoolPlayer> pool(2);

    pool.resize(5);

    auto object = pool.acquire(100, "AfterResize");

    assert(object->getHP() == 100);
  });

  // RESIZE WITH ACTIVE OBJECTS
  runner.add("Pool", "ResizeWithObjectsThrows", []() {
    Pool<PoolPlayer> pool(2);

    auto object = pool.acquire(100, "Active");

    bool thrown = false;

    try {
      pool.resize(5);
    } catch (const std::logic_error&) {
      thrown = true;
    }

    assert(thrown);
  });

  // REUSE AFTER RELEASE
  runner.add("Pool", "ReuseReleasedMemory", []() {
    Pool<PoolPlayer> pool(1);

    {
      auto object = pool.acquire(10, "Old");
    }

    auto object = pool.acquire(999, "New");

    assert(object->getHP() == 999);
  });

}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Pool(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif

