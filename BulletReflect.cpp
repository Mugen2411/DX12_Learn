#include "BulletReflect.h"

#include "Constant.h"

namespace bullet {

BulletReflect::BulletReflect(std::string gid, int gnum, float x, float y,
                             float a, float sp)
    : BulletBase(gid, gnum, x, y, a, sp), _is_reflected(false) {}

BulletBase::State BulletReflect::Update() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  if (!_is_reflected) {
    if (_x < Constant::kGameTopX) {
      _x += (Constant::kGameTopX - _x) * 2;
      _a = Constant::kPI - _a;
      _is_reflected = true;
    }
    if (_x > Constant::kGameBottomX) {
      _x += (Constant::kGameBottomX - _x) * 2;
      _a = Constant::kPI - _a;
      _is_reflected = true;
    }
    if (_y < Constant::kGameTopY) {
      _y += (Constant::kGameTopY - _y) * 2;
      _a *= -1;
      _is_reflected = true;
    }
  }

  if (_x < Constant::kGameTopX || _x > Constant::kGameBottomX || _y < Constant::kGameTopY ||
      _y > Constant::kGameBottomY)
    _state = State::kDead;
  return _state;
}
}  // namespace bullet