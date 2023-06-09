#pragma once
#include "BulletBase.h"
#include "BulletAccel.h"
#include "BulletReflect.h"

namespace bullet {
namespace Descriptor {
struct ACCEL_REFLECT_DESC : virtual public ACCEL_DESC, REFLECT_DESC {};
}  // namespace Descriptor
class BulletAccelReflect : virtual public BulletAccel, BulletReflect {
 public:
  BulletAccelReflect(Descriptor::ACCEL_REFLECT_DESC &desc);

 protected:
  virtual State Update();
  void Process();
};

std::shared_ptr<BulletBase> Create(Descriptor::ACCEL_REFLECT_DESC desc);
}  // namespace bullet