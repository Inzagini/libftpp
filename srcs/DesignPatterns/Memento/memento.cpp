#include "DesignPatterns/Memento/memento.hpp"

Memento::Snapshot Memento::save() {
  Snapshot snapshot;
  _saveToSnapshot(snapshot);

  return snapshot;
}

void Memento::load(const Snapshot& snapshot) {
  _loadFromSnapshot(const_cast<Snapshot&>(snapshot));
}
