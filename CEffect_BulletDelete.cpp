#include "CEffect_BulletDelete.h"

CEffect_BulletDelete::CEffect_BulletDelete(CVector position_, CVector velocity,
                                           double size, int color)
    : CEffect(position_),
      velocity_(velocity * 1.0),
      size_(size),
      color_(color),
      cnt_(0) {
  _graph = mugen_engine::getIns().LoadGraphic("eff_bulletdelete");
}

void CEffect_BulletDelete::Update() {
  position_ += velocity_;
  cnt_++;
  if (cnt_ / 3.0 > 7) status_ = 1;
}

void CEffect_BulletDelete::Render() const {
  _graph->Draw(position_.x_, position_.y_, velocity_.GetAngle(),
                    size_ / 32.0, Constant::Priority::kEffect,
               (uint32_t)(cnt_ / 3.0), 0, mugen_engine::MainColor[color_]);
  _graph->Draw(position_.x_, position_.y_, velocity_.GetAngle(),
                    size_ / 32.0,
               Constant::Priority::kEffect,
               (uint32_t)(cnt_ / 3.0), 1,
               mugen_engine::Graphic::Color{1.0f, 1.0f, 1.0f, 1.0f});
}
