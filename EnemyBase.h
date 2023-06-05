#pragma once
#include <string>

#include "Mover.h"

namespace enemy {

namespace Descriptor {
struct BASE_DESC {
  float x, y;
  std::string gid;
};
}  // namespace Descriptor

class EnemyBase : public mover::Mover {
 public:
  EnemyBase();
  virtual ~EnemyBase();
  State Update();
  void Render() const;

  void Dead();
  void Disappear();

 protected:
  std::string gid_;
};

}  // namespace enemy
