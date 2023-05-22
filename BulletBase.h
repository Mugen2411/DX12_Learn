#pragma once
#include <string>

#include "DxGraphic.h"
#include "Engine.h"
#include "Mover.h"
#include "CEffect_BulletDelete.h"
#include "CEffectParent.h"

namespace bullet {
class BulletBase : public mover::Mover {
 public:
  BulletBase(std::string gid, int gnum, float x, float y, float a, float sp);

 protected:
  float _x, _y;
  float _a, _sp;
  std::shared_ptr<mugen_engine::Graphic> _graph;
  int _gnum;

  virtual State Update();
  virtual void Render() const;

  void Dead() {
    CEffectParent::RegisterEffect(std::make_shared<CEffect_BulletDelete>(
        CVector(_x, _y), CVector(_a) * _sp, 32, _gnum%8));
  };
  void Disappear(){};
};
}  // namespace bullet