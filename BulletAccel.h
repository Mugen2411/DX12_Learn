#pragma once
#include "BulletBase.h"

namespace bullet {
namespace Descriptor {
struct ACCEL_DESC : virtual public BASE_DESC {
  float angle_accel;
  float speed_accel;
  int accel_time;
};
}  // namespace Descriptor
class BulletAccel : virtual public BulletBase {
 public:
  BulletAccel(Descriptor::ACCEL_DESC &desc);

 protected:
  float _angle_accel, _speed_accel;
  int _accel_time;
  void Process();
  virtual State Update();
};

std::shared_ptr<BulletBase> Create(Descriptor::ACCEL_DESC desc);
}  // namespace bullet