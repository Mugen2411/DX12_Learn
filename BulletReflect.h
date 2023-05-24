#pragma once
#include "BulletBase.h"

namespace bullet {
namespace Descriptor {
struct REFLECT_DESC : virtual public BASE_DESC {};
}  // namespace Descriptor
class BulletReflect : virtual public BulletBase {
 public:
  BulletReflect(Type type, Color color, float x, float y, float a, float sp,
                BlendType blendtype);

 protected:
  bool _is_reflected;
  virtual State Update();
  int Reflect();
};

std::shared_ptr<BulletBase> Create(Descriptor::REFLECT_DESC desc);
}  // namespace bullet