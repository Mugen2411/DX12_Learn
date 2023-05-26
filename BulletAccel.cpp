#include "BulletAccel.h"

namespace bullet {
std::shared_ptr<BulletBase> bullet::Create(Descriptor::ACCEL_DESC desc) {
  return std::make_shared<BulletAccel>(desc);
}

BulletAccel::BulletAccel(Descriptor::ACCEL_DESC &desc)
    : BulletBase(desc),
      _angle_accel(desc.angle_accel),
      _speed_accel(desc.speed_accel),
      _accel_time(desc.accel_time) {}
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