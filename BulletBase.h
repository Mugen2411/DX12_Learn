#pragma once
#include <string>

#include "DxGraphic.h"
#include "Engine.h"
#include "Mover.h"

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

  void Dead(){};
  void Disappear(){};
};
}  // namespace bullet