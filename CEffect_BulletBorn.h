#pragma once
#include "CEffect.h"
#include "DxGraphic.h"
#include "Engine.h"

class CEffect_BulletBorn : public CEffect {
 public:
  CEffect_BulletBorn(CVector position_, CVector velocity, double size,
                       int color);

  void Update();
  void Render() const;

 private:
  CVector velocity_;
  int cnt_ = 0;
  float duration_ = 2.0f;
  double size_;
  int color_;
  std::shared_ptr<mugen_engine::Graphic> _graph;
};
