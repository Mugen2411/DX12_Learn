#pragma once
#include "MoverManager.h"

namespace mover {
class Mover {
 public:
  enum class State { kAlive, kDead, kDisappear };
  Mover() : _state(State::kAlive) {}

  virtual State Update() = 0;
  virtual void Render() const = 0;
  virtual void Dead() = 0;
  virtual void Disappear() = 0;

 protected:
  State _state;

 private:

  friend Manager;
};
}  // namespace mover