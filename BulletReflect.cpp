#include "BulletReflect.h"

#include "Constant.h"

namespace bullet {

BulletReflect::BulletReflect(Type type, Color color, float x, float y,
                             float a, float sp, BlendType blendtype)
    : BulletBase(type, color, x, y, a, sp, blendtype), _is_reflected(false) {}

BulletBase::State BulletReflect::Update() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  Reflect();
  OutJudge();
  return _state;
}
void BulletReflect::Reflect() {
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
}
std::shared_ptr<BulletBase> Create(Descriptor::REFLECT_DESC desc) {
  return std::make_shared<BulletReflect>(desc.type, desc.color,
      desc.x, desc.y, desc.angle, desc.speed, desc.blendtype);
}
}  // namespace bullet