#pragma once
#include "CEffect.h"
#include "Engine.h"
#include "DxGraphic.h"

class CEffect_BulletDelete : public CEffect {
 public:
  CEffect_BulletDelete(CVector position_, CVector velocity, double size,
                       int color);

  void Update();
  void Render() const;

 private:
  CVector velocity_;
  int cnt_ = 0;
  double size_;
  int color_;
  std::shared_ptr<mugen_engine::Graphic> _graph;
};
