#pragma once
#include "MoverManager.h"
namespace bullet {
class BulletBase;
}
class Player;

namespace mover {
class Mover {
 public:
  enum class State { kAlive, kDead, kDisappear };
  Mover(float x, float y, float a, float sp, int radius)
      : _state(State::kAlive), _x(x), _y(y), _a(a), _sp(sp), _radius(radius) {}

  virtual State Update() = 0;
  virtual void Render() const = 0;
  virtual void Dead() = 0;
  virtual void Disappear() = 0;

  bool Collision(Mover* m);
  virtual void Dispatch(Mover* m) = 0;
  virtual void Hit(Mover* m) {}
  virtual void Hit(bullet::BulletBase* m) {}
  virtual void Hit(Player* m) {}

 protected:
  State _state;
  float _x, _y;
  float _a, _sp;
  int _radius;

  friend Manager;
};
}  // namespace mover