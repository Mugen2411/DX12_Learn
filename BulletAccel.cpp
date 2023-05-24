#include "BulletAccel.h"

namespace bullet {
std::shared_ptr<BulletBase> bullet::Create(Descriptor::ACCEL_DESC desc) {
  return std::make_shared<BulletAccel>(
      desc.type, desc.color, desc.x, desc.y, desc.angle, desc.angle_accel,
      desc.accel_time, desc.speed, desc.speed_accel, desc.blendtype);
}

BulletAccel::BulletAccel(Type type, Color color, float x, float y, float a,
                         float aa, int accel_time, float sp, float spa,
                         BlendType blendtype)
    : BulletBase(type, color, x, y, a, sp, blendtype),
      _angle_accel(aa),
      _speed_accel(spa),
      _accel_time(accel_time) {}
void BulletAccel::Process() {
  if (_cnt < _accel_time) {
    _a += _angle_accel;
    _sp += _speed_accel;
  }
  BulletBase::Process();
}
BulletBase::State BulletAccel::Update() {
  Process();
  OutJudge();
  return _state;
}
}  // namespace bullet