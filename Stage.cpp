#include "Stage.h"

#include "BulletAccelReflect.h"
#include "Constant.h"
#include "MoverManager.h"
#include "Player.h"

Stage::Stage() : _cnt(0) {}

void Stage::Initialize() {
  mover::Manager::getIns().RegisterBullet(std::make_shared<mover::Player>(),
                                          false);
  _frame = mugen_engine::getIns().LoadGraphic("sys_frame");
}

void Stage::Update() {
  static float base_angle = 0.0f;
  static float angle_accel = Constant::kPI2 / 4.0f / 120.0f;
  static int color = 0;
  if (_cnt % 30 == 0) {
    bullet::Descriptor::ACCEL_REFLECT_DESC desc = {};
    desc.x = Constant::kGameWidth / 2 + Constant::kGameTopX;
    desc.y = Constant ::kGameHeight / 4 + Constant::kGameTopY;
    desc.color = static_cast<bullet::Color>((++color) %= 8);
    desc.type = bullet::Type::kRing;
    desc.speed = 4.0f;
    desc.speed_accel = -1.8f / 120.0f;
    desc.angle = base_angle += Constant::kPI2 / 120.0f;
    desc.angle_accel = angle_accel;
    desc.accel_time = 120;
    desc.blendtype = bullet::BlendType::kNone;
    for (int i = 0; i < 8; i++) {
      desc.angle += Constant::kPI2 / 8.0f;
      mover::Manager::getIns().RegisterBullet(bullet::Create(desc), true);
      // desc.angle_accel *= -1.0f;
      //  mover::Manager::getIns().RegisterBullet(bullet::Create(desc), true);
      //  desc.angle_accel *= -1.0f;
    }
    angle_accel *= -1.0f;
  }
  mover::Manager::getIns().Update();
  _cnt++;
}

void Stage::Render() const {
  mover::Manager::getIns().Render();
  _frame->Draw(Constant::kScreenWidth / 2.0f, Constant::kScreenHeight / 2.0f,
               0.0f, 1.0f, 0.0f);
}
