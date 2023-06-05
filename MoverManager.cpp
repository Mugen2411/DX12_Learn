#include "MoverManager.h"

#include "Mover.h"
#include "Player.h"
#include "BulletBase.h"

namespace mover {

Manager Manager::_instance;

void Manager::RegisterBullet(std::shared_ptr<Mover> m,
                             bool isDependOnParent) {
  if (isDependOnParent) bullet_list_.Register(m, enemy_list_.GetCurrentIndex());
  else bullet_list_.Register(m, -1);
}

void Manager::RegisterEnemy(std::shared_ptr<Mover> m, bool isDependOnParent) {
  if (isDependOnParent) enemy_list_.Register(m, enemy_list_.GetCurrentIndex());
  else enemy_list_.Register(m, -1);
}

void Manager::Update() {
  player_->Update();
  enemy_list_.Update();
  bullet_list_.Update();
  bullet_list_.Dispatch(player_);
}

void Manager::Render() const {
  enemy_list_.Render();
  bullet_list_.Render();
  player_->Render();
}

Manager::Manager()
    : kMaxIndex(16384), bullet_list_(kMaxIndex), enemy_list_(kMaxIndex){
}
void List::Register(std::shared_ptr<Mover> m, int parent) {
  if (unused_index_.empty()) return;
  int idx = unused_index_.front();
  unused_index_.pop();
  list_[idx].alive = true;
  list_[idx].contents.swap(m);
  list_[idx].parent = parent;
}
void List::Update() {
  while (!dead_index_.empty()) {
    unused_index_.push(dead_index_.front());
    dead_index_.pop();
  }
  for (currentIndex_ = 0; currentIndex_ < kMaxIndex;
       currentIndex_++) {
    if (!list_[currentIndex_].alive) continue;
    list_[currentIndex_].contents->Update();
  }
  for (currentIndex_ = 0; currentIndex_ < kMaxIndex;
       currentIndex_++) {
    if (!list_[currentIndex_].alive) continue;
    if (list_[currentIndex_].contents->_state !=
        mover::Mover::State::kAlive) {
      list_[currentIndex_].alive = false;
      list_[currentIndex_].parent = -1;
      dead_index_.push(currentIndex_);
      if (list_[currentIndex_].contents->_state ==
          mover::Mover::State::kDead)
        list_[currentIndex_].contents->Dead();
      if (list_[currentIndex_].contents->_state ==
          mover::Mover::State::kDisappear)
        list_[currentIndex_].contents->Disappear();
    }
  }
  currentIndex_ = -1;
}
void List::Render() const {
  for (int i = 0; i < kMaxIndex; i++) {
    if (!list_[i].alive) continue;
    list_[i].contents->Render();
  }
}
void List::Dispatch(std::shared_ptr<Mover> m) {
  for (int i = 0; i < kMaxIndex; i++) {
    if (!list_[i].alive) continue;
    list_[i].contents->Dispatch(m.get());
    m->Dispatch(list_[i].contents.get());
  }
}
}  // namespace mover