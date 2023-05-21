#include "MoverManager.h"

#include "Mover.h"

namespace mover {

Manager Manager::_instance;
void Manager::Update() {
  while (!_dead_index.empty()) {
    _unused_index.push(_dead_index.front());
    _dead_index.pop();
  }
  for (_currentIndex = 0; _currentIndex < kMaxIndex; _currentIndex++) {
    if (!_mover_list[_currentIndex].alive) continue;
    _mover_list[_currentIndex].contents->Update();
  }
  for (_currentIndex = 0; _currentIndex < kMaxIndex; _currentIndex++) {
    if (!_mover_list[_currentIndex].alive) continue;
    if (_mover_list[_currentIndex].contents->_state !=
        mover::Mover::State::kAlive) {
      _mover_list[_currentIndex].alive = false;
      _dead_index.push(_currentIndex);
    }
  }
}

void Manager::Render() const {
  for (int i = 0; i < kMaxIndex; i++) {
    if (!_mover_list[i].alive) continue;
    _mover_list[i].contents->Render();
  }
}

Manager::Manager() : kMaxIndex(65536), _mover_list(kMaxIndex) {
  for (int i = 0; i < kMaxIndex; i++) {
    _unused_index.push(i);
  }
}
}  // namespace mover