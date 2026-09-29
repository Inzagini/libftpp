#include "DesignPatterns/Memento/memento.hpp"
#include "Tester/runner.hpp"

#include <cassert>
#include <string>

class MementoPlayer : public Memento {

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
  MementoPlayer(int h = 100, int p = 0) : hp(h), position(p) {}

  void damage(int amount) { hp -= amount; }

  void move(int amount) { position += amount; }

  int getHP() const { return hp; }

  int getPosition() const { return position; }
};

void register_Memento(Test::Runner& runner) {

  /*
      BASIC SAVE / LOAD
  */

  runner.add("Memento", "SaveAndRestore", []() {
    MementoPlayer player(100, 10);

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
    MementoPlayer player(100, 10);

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
    MementoPlayer player(100, 0);

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
    MementoPlayer player;

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
    MementoPlayer player;

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
    MementoPlayer player;

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

}

#ifndef LIBFTPP_TEST_NO_MAIN
int main(int argc, char** argv) {
  Test::Runner runner;
  register_Memento(runner);
  return runner.run(argc > 1 ? argv[1] : "");
}
#endif

