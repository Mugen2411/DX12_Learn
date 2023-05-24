#include "BulletAccelReflect.h"

namespace bullet {
BulletAccelReflect::BulletAccelReflect(Type type, Color color, float x, float y,
                                       float a, float aa, int accel_time,
                                       float sp, float spa, BlendType blendtype)
    : BulletAccel(type, color, x, y, a, aa, accel_time, sp, spa, blendtype),
      BulletReflect(type, color, x, y, a, sp, blendtype),
      BulletBase(type, color, x, y, a, sp, blendtype) {}
BulletBase::State BulletAccelReflect::Update() {
  Process();
  Reflect();
  OutJudge();
  return _state;
}
void BulletAccelReflect::Process() {
  BulletAccel::Process();
}
std::shared_ptr<BulletBase> Create(Descriptor::ACCEL_REFLECT_DESC desc) {
  return std::make_shared<BulletAccelReflect>(
      desc.type, desc.color, desc.x, desc.y, desc.angle, desc.angle_accel,
      desc.accel_time, desc.speed, desc.speed_accel, desc.blendtype);
  ;
}
}  // namespace bullet