#include "Stage.h"

#include "BulletReflect.h"
#include "Constant.h"
#include "MoverManager.h"

Stage::Stage() : _cnt(0) {}

void Stage::Update() {
  if (_cnt % 90 == 0) {
    int x = rand() % 180;
    int y = rand() % 180;
    int col = rand() % 8;
    float a = Constant::kPI / 64 * (rand() % 64);
    for (int i = 0; i < 32; i++) {
      mover::Manager::getIns().addMover(std::make_shared<bullet::BulletReflect>(
          "B_small", 8 + col,
          (Constant::kGameBottomX - Constant::kGameTopX) / 2 - 90 + x,
          Constant::kScreenHeight / 4 - 90 + y, a + Constant::kPI2 / 32 * i,
          2.0f));
    }
  }
  mover::Manager::getIns().Update();
  _cnt++;
}

void Stage::Render() const { mover::Manager::getIns().Render(); }
