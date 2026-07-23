#pragma once

#include <any>
#include <string>
#include <unordered_map>
class Memento {

public:
  using Snapshot = std::unordered_map<std::string, std::any>;

  Snapshot save() {
    Snapshot snapshot;
    _saveToSnapshot(snapshot);

    return snapshot;
  }

  void load(const Snapshot& snapshot) {
    _loadFromSnapshot(const_cast<Snapshot&>(snapshot));
  }

private:
  virtual void _saveToSnapshot(Snapshot&) const = 0;
  virtual void _loadFromSnapshot(Snapshot&) = 0;
};
