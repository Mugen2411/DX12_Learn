#include "CEffect_BulletBorn.h"

CEffect_BulletBorn::CEffect_BulletBorn(CVector position_, CVector velocity,
                                       double size, int color)
    : CEffect(position_),
      velocity_(velocity * 1.0),
      size_(size),
      color_(color),
      cnt_(0) {
  _graph = mugen_engine::getIns().LoadGraphic("eff_bulletdelete");
}

void CEffect_BulletBorn::Update() {
  position_ += velocity_;
  cnt_++;
  if (cnt_ / duration_ > 7) status_ = 1;
}

void CEffect_BulletBorn::Render() const {
  auto c = mugen_engine::MainColor[color_];
  c.blendLevel = (1.0f - (cnt_ / 7.0f / duration_));
  _graph->Draw(position_.x_, position_.y_, velocity_.GetAngle(),
               (size_ / 24.0) * (1.0f - cnt_ / 7.0f / duration_),
               Constant::Priority::kEffect, 2, 0, c);
  _graph->Draw(
      position_.x_, position_.y_, velocity_.GetAngle(),
      (size_ / 32.0) * (1.0f - cnt_ / 7.0f / duration_),
      Constant::Priority::kEffect, 2, 1,
      mugen_engine::Graphic::Color{1.0f, 1.0f, 1.0f,
                                   (1.0f - (cnt_ / 7.0f / duration_)) * 0.7f});
}
