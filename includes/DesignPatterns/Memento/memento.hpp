#pragma once

#include <any>
#include <string>
#include <unordered_map>
class Memento {

public:
  using Snapshot = std::unordered_map<std::string, std::any>;

  Snapshot save();
  void load(const Snapshot& snapshot);

private:
  virtual void _saveToSnapshot(Snapshot&) const = 0;
  virtual void _loadFromSnapshot(Snapshot&) = 0;
};
