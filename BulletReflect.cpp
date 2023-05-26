#include "BulletReflect.h"

#include "Constant.h"

namespace bullet {

BulletReflect::BulletReflect(Descriptor::REFLECT_DESC &desc)
    : BulletBase(desc), _is_reflected(false) {}

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
      ret = 1;
    }
  }
  return ret;
}
std::shared_ptr<BulletBase> Create(Descriptor::REFLECT_DESC desc) {
  return std::make_shared<BulletReflect>(desc);
}
}  // namespace bullet