#include "Stage.h"

#include "BulletReflect.h"
#include "Constant.h"
#include "MoverManager.h"

Stage::Stage() : _cnt(0) {}

void Stage::Update() {
  if (_cnt % 30 == 0) {
    int x = rand() % 180;
    int y = rand() % 180;
    int col = rand() % 8;
    float a = Constant::PI / 64 * (rand() % 64);
    for (int i = 0; i < 128; i++) {
      mover::Manager::getIns().addMover(std::make_shared<bullet::BulletReflect>(
          "B_small", 8 + col,
          (Constant::game_bottom_x - Constant::game_top_x) / 2 - 90 + x,
          Constant::screen_height / 4 - 90 + y, a + Constant::PI2 / 128 * i,
          3.5f));
    }
  }
  mover::Manager::getIns().Update();
  _cnt++;
}

void Stage::Render() const { mover::Manager::getIns().Render(); }
