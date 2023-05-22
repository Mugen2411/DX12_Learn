#include "Stage.h"

#include "BulletReflect.h"
#include "Constant.h"
#include "MoverManager.h"
#include "Player.h"

Stage::Stage() : _cnt(0) {}

void Stage::Initialize() {
  mover::Manager::getIns().addMover(std::make_shared<mover::Player>(), false);
  _frame = mugen_engine::getIns().LoadGraphic("sys_frame");
}

void Stage::Update() {
  if (_cnt % 90 == 0) {
    int x = rand() % 180;
    int y = rand() % 180;
    int col = rand() % 8;
    float a = Constant::kPI / 64 * (rand() % 64);
    for (int i = 0; i < 32; i++) {
      mover::Manager::getIns().addMover(
          std::make_shared<bullet::BulletReflect>(
              "B_small", 24 + col,
              (static_cast<float>(Constant::kGameBottomX -
                                  Constant::kGameTopX) /
                   2 -
               90 + x),
              static_cast<float>(Constant::kScreenHeight / 4 - 90 + y),
              a + Constant::kPI2 / 32 * i, 2.0f),
          true);
    }
  }
  mover::Manager::getIns().Update();
  _cnt++;
}

void Stage::Render() const { mover::Manager::getIns().Render();
  _frame->Draw(Constant::kScreenWidth / 2.0f, Constant::kScreenHeight / 2.0f,
               0.0f, 1.0f, 0.0f);
}
