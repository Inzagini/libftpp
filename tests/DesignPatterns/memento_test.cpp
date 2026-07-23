#include "DesignPatterns/Memento/memento.hpp"

#include "../utils/test_runner.hpp"

#include <cassert>
#include <string>

class Player : public Memento {

private:
  int hp = 100;
  int position = 0;

  void _saveToSnapshot(Snapshot& snapshot) const override {
    snapshot["hp"] = hp;
    snapshot["position"] = position;
  }

  void _loadFromSnapshot(Snapshot& snapshot) override {
    hp = std::any_cast<int>(snapshot.at("hp"));
    position = std::any_cast<int>(snapshot.at("position"));
  }

public:
  Player(int h = 100, int p = 0) : hp(h), position(p) {}

  void damage(int amount) { hp -= amount; }

  void move(int amount) { position += amount; }

  int getHP() const { return hp; }

  int getPosition() const { return position; }
};

int main() {
  TestRunner runner;

  /*
      BASIC SAVE / LOAD
  */

  runner.add("Memento", "SaveAndRestore", []() {
    Player player(100, 10);

    auto snapshot = player.save();

    player.damage(50);
    player.move(20);

    assert(player.getHP() == 50);
    assert(player.getPosition() == 30);

    player.load(snapshot);

    assert(player.getHP() == 100);
    assert(player.getPosition() == 10);
  });

  /*
      SNAPSHOT IS INDEPENDENT
  */

  runner.add("Memento", "SnapshotIndependence", []() {
    Player player(100, 10);

    auto snapshot = player.save();

    player.damage(90);

    assert(player.getHP() == 10);

    player.load(snapshot);

    assert(player.getHP() == 100);
  });

  /*
      MULTIPLE SNAPSHOTS
  */

  runner.add("Memento", "MultipleSnapshots", []() {
    Player player(100, 0);

    auto first = player.save();

    player.damage(20);

    auto second = player.save();

    player.damage(30);

    assert(player.getHP() == 50);

    player.load(second);

    assert(player.getHP() == 80);

    player.load(first);

    assert(player.getHP() == 100);
  });

  /*
      EMPTY SNAPSHOT
  */

  runner.add("Memento", "EmptySnapshotThrows", []() {
    Player player;

    Memento::Snapshot empty;

    bool thrown = false;

    try {
      player.load(empty);
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      WRONG TYPE IN SNAPSHOT
  */

  runner.add("Memento", "WrongTypeThrows", []() {
    Player player;

    Memento::Snapshot snapshot;

    snapshot["hp"] = std::string("wrong");

    snapshot["position"] = 10;

    bool thrown = false;

    try {
      player.load(snapshot);
    } catch (const std::bad_any_cast&) {
      thrown = true;
    }

    assert(thrown);
  });

  /*
      MISSING KEY
  */

  runner.add("Memento", "MissingKeyThrows", []() {
    Player player;

    Memento::Snapshot snapshot;

    snapshot["hp"] = 50;

    bool thrown = false;

    try {
      player.load(snapshot);
    } catch (const std::exception&) {
      thrown = true;
    }

    assert(thrown);
  });

  return runner.run();
}
