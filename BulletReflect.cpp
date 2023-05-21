#include "BulletReflect.h"

#include "Constant.h"

namespace bullet {

BulletReflect::BulletReflect(std::string gid, int gnum, float x, float y,
                             float a, float sp)
    : BulletBase(gid, gnum, x, y, a, sp) {}

BulletBase::State BulletReflect::Update() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  if (_x < Constant::game_top_x) {
    _x += (Constant::game_top_x - _x) * 2;
    _a = Constant::PI - _a;
  }
  if (_x > Constant::game_bottom_x) {
    _x += (Constant::game_bottom_x - _x) * 2;
    _a = Constant::PI - _a;
  }
  if (_y < Constant::game_top_y) {
    _x += (Constant::game_top_y - _y) * 2;
    _a *= -1;
  }

  if (_x < 0 || _x > Constant::screen_width || _y < 0 ||
      _y > Constant::screen_height)
    _state = State::kDead;
  return _state;
}
}  // namespace bullet