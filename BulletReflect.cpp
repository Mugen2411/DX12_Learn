#include "BulletReflect.h"

#include "Constant.h"

namespace bullet {

BulletReflect::BulletReflect(Type type, Color color, float x, float y, float a,
                             float sp, BlendType blendtype)
    : BulletBase(type, color, x, y, a, sp, blendtype), _is_reflected(false) {}

BulletBase::State BulletReflect::Update() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  Reflect();
  OutJudge();
  return _state;
}
int BulletReflect::Reflect() {
  int ret = 0;
  if (!_is_reflected) {
    if (_x < Constant::kGameTopX) {
      _x += (Constant::kGameTopX - _x) * 2;
      _a = Constant::kPI - _a;
      _is_reflected = true;
      ret = 1;
    }
    if (_x > Constant::kGameBottomX) {
      _x += (Constant::kGameBottomX - _x) * 2;
      _a = Constant::kPI - _a;
      _is_reflected = true;
      ret = 1;
    }
    if (_y < Constant::kGameTopY) {
      _y += (Constant::kGameTopY - _y) * 2;
      _a *= -1;
      _is_reflected = true;
      ret = 2;
    }
    return ret;
  }
  return ret;
}
std::shared_ptr<BulletBase> Create(Descriptor::REFLECT_DESC desc) {
  return std::make_shared<BulletReflect>(desc.type, desc.color, desc.x, desc.y,
                                         desc.angle, desc.speed,
                                         desc.blendtype);
}
}  // namespace bullet