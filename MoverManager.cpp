#include "MoverManager.h"

#include "Mover.h"
#include "Player.h"
#include "BulletBase.h"

namespace mover {

Manager Manager::_instance;

void Manager::RegisterBullet(std::shared_ptr<Mover> m,
                             bool isDependOnParent) {
  if (unused_bullet_index_.empty()) return;
  int idx = unused_bullet_index_.front();
  unused_bullet_index_.pop();
  bullet_list_[idx].alive = true;
  bullet_list_[idx].contents.swap(m);
  if (isDependOnParent) bullet_list_[idx].parent = currentEnemyIndex_;
}

void Manager::Update() {
  while (!dead_bullet_index_.empty()) {
    unused_bullet_index_.push(dead_bullet_index_.front());
    dead_bullet_index_.pop();
  }
  player_->Update();
  for (currentEnemyIndex_ = 0; currentEnemyIndex_ < kMaxIndex; currentEnemyIndex_++) {
    if (!bullet_list_[currentEnemyIndex_].alive) continue;
    bullet_list_[currentEnemyIndex_].contents->Update();
  }
  for (int i = 0; i < kMaxIndex; i++) {
    if (!bullet_list_[i].alive) continue;
    bullet_list_[i].contents->Dispatch(player_.get());
    player_->Dispatch(bullet_list_[i].contents.get());
  }
  for (currentEnemyIndex_ = 0; currentEnemyIndex_ < kMaxIndex; currentEnemyIndex_++) {
    if (!bullet_list_[currentEnemyIndex_].alive) continue;
    if (bullet_list_[currentEnemyIndex_].contents->_state !=
        mover::Mover::State::kAlive) {
      bullet_list_[currentEnemyIndex_].alive = false;
      bullet_list_[currentEnemyIndex_].parent = -1;
      dead_bullet_index_.push(currentEnemyIndex_);
      if (bullet_list_[currentEnemyIndex_].contents->_state ==
          mover::Mover::State::kDead)
        bullet_list_[currentEnemyIndex_].contents->Dead();
      if (bullet_list_[currentEnemyIndex_].contents->_state ==
          mover::Mover::State::kDisappear)
        bullet_list_[currentEnemyIndex_].contents->Disappear();
    }
  }
  currentEnemyIndex_ = -1;
}

void Manager::Render() const {
  for (int i = 0; i < kMaxIndex; i++) {
    if (!bullet_list_[i].alive) continue;
    bullet_list_[i].contents->Render();
  }
  player_->Render();
}

Manager::Manager()
    : kMaxIndex(16384), bullet_list_(kMaxIndex), currentEnemyIndex_(-1) {
  for (int i = 0; i < kMaxIndex; i++) {
    unused_bullet_index_.push(i);
  }
}
}  // namespace mover