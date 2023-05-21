#pragma once
#include "BulletBase.h"

namespace bullet {
class BulletReflect : public BulletBase {
 public:
  BulletReflect(std::string gid, int gnum, float x, float y, float a, float sp);

 protected:
  virtual State Update();
};
}  // namespace bullet