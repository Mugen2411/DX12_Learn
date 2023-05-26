#include "MoverManager.h"

#include "Mover.h"
#include "Player.h"

namespace mover {

Manager Manager::_instance;
void Manager::Update() {
  while (!_dead_index.empty()) {
    _unused_index.push(_dead_index.front());
    _dead_index.pop();
  }
  _player->Update();
  for (_currentIndex = 0; _currentIndex < kMaxIndex; _currentIndex++) {
    if (!_mover_list[_currentIndex].alive) continue;
    _mover_list[_currentIndex].contents->Update();
  }
  for (int i = 0; i < kMaxIndex; i++) {
    if (!_mover_list[i].alive) continue;
    _mover_list[i].contents->Dispatch(_player.get());
    _player->Dispatch(_mover_list[i].contents.get());
  }
  for (_currentIndex = 0; _currentIndex < kMaxIndex; _currentIndex++) {
    if (!_mover_list[_currentIndex].alive) continue;
    if (_mover_list[_currentIndex].contents->_state !=
        mover::Mover::State::kAlive) {
      _mover_list[_currentIndex].alive = false;
      _mover_list[_currentIndex].parent = -1;
      _dead_index.push(_currentIndex);
      if (_mover_list[_currentIndex].contents->_state ==
          mover::Mover::State::kDead)
        _mover_list[_currentIndex].contents->Dead();
      if (_mover_list[_currentIndex].contents->_state ==
          mover::Mover::State::kDisappear)
        _mover_list[_currentIndex].contents->Disappear();
    }
  }
  _currentIndex = -1;
}

void Manager::Render() const {
  for (int i = 0; i < kMaxIndex; i++) {
    if (!_mover_list[i].alive) continue;
    _mover_list[i].contents->Render();
  }
  _player->Render();
}

Manager::Manager()
    : kMaxIndex(16384), _mover_list(kMaxIndex), _currentIndex(-1) {
  for (int i = 0; i < kMaxIndex; i++) {
    _unused_index.push(i);
  }
}
}  // namespace mover