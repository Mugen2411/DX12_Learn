#include "BulletAccelReflect.h"

namespace bullet {
BulletAccelReflect::BulletAccelReflect(Descriptor::ACCEL_REFLECT_DESC &desc)
    : BulletAccel(desc),
      BulletReflect(desc),
      BulletBase(desc) {}
BulletBase::State BulletAccelReflect::Update() {
  Process();
  if (Reflect() == 1) _angle_accel *= -1.0f;
  OutJudge();
  return _state;
}
void BulletAccelReflect::Process() { BulletAccel::Process(); }
std::shared_ptr<BulletBase> Create(Descriptor::ACCEL_REFLECT_DESC desc) {
  return std::make_shared<BulletAccelReflect>(desc);
  ;
}
}  // namespace bullet