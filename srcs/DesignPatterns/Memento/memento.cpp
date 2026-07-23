#include "DesignPatterns/Memento/memento.hpp"

// test case

class Player : public Memento {

  friend class Memento;

private:
  int hp = 100;

  void _saveToSnaphot(Snapshot& snapshot) const { snapshot["hp"] = hp; }

  void _loadFromSnapshot(Snapshot& snapshot) {
    hp = std::any_cast<int>(snapshot["hp"]);
  }
};
