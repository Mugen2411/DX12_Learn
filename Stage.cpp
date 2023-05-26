#include "Stage.h"

#include "BulletAccelReflect.h"
#include "Constant.h"
#include "MoverManager.h"
#include "Player.h"

Stage::Stage() : _cnt(0) {}

void Stage::Initialize() {
  mover::Manager::getIns().addMover(std::make_shared<mover::Player>(), false);
  _frame = mugen_engine::getIns().LoadGraphic("sys_frame");
}

void Stage::Update() {
  if (_cnt % 120 == 0) {
    bullet::Descriptor::ACCEL_REFLECT_DESC desc = {};
    desc.x = -180 + util::Random::getIns().getRand(360) +
             Constant::kGameWidth / 2 + Constant::kGameTopX;
    desc.y = -180 + util::Random::getIns().getRand(360) +
             Constant ::kGameHeight / 4 + Constant::kGameTopY;
    desc.color = static_cast<bullet::Color>(util::Random::getIns().getRand(8));
    desc.type = bullet::Type::kArrow;
    desc.speed = 3.0f;
    desc.speed_accel = -1.8f / 120.0f;
    desc.angle = Constant::kPI / 256 * (util::Random::getIns().getRand(256));
    desc.angle_accel = Constant::kPI2 / 4.0f / 120.0f;
    desc.accel_time = 120;
    desc.blendtype = static_cast<bullet::BlendType>(util::Random::getIns().getRand(2));
    for (int i = 0; i < 32; i++) {
      desc.angle += Constant::kPI2 / 32.0f;
      mover::Manager::getIns().addMover(bullet::Create(desc), true);
      desc.angle_accel *= -1.0f;
      mover::Manager::getIns().addMover(bullet::Create(desc), true);
      desc.angle_accel *= -1.0f;
    }
  }
  mover::Manager::getIns().Update();
  _cnt++;
}

void Stage::Render() const {
  mover::Manager::getIns().Render();
  _frame->Draw(Constant::kScreenWidth / 2.0f, Constant::kScreenHeight / 2.0f,
               0.0f, 1.0f, 0.0f);
}
