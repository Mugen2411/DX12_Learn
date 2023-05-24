#pragma once
#include <memory>
#include <queue>
#include <vector>

namespace bullet {
class BulletBase;
}
namespace mover {
class Mover;
class Player;
struct MOVER_ELEM {
  bool alive = false;
  int parent = -1;
  std::shared_ptr<Mover> contents;
};
class Manager {
 public:
  static Manager& getIns() { return _instance; }
  const int kMaxIndex;
  void addMover(std::shared_ptr<Mover> m, bool isDependOnParent) {
    if (_unused_index.empty()) return;
    int idx = _unused_index.front();
    _unused_index.pop();
    _mover_list[idx].alive = true;
    _mover_list[idx].contents.swap(m);
    if (isDependOnParent) _mover_list[idx].parent = _currentIndex;
  }
  void addMover(std::shared_ptr<Player> p, bool isDependOnParent) {
    _player.swap(p);
  }
  void Update();
  void Render() const;

 private:
  static Manager _instance;
  Manager();
  std::vector<MOVER_ELEM> _mover_list;
  std::shared_ptr<Player> _player;
  std::queue<int> _unused_index;
  std::queue<int> _dead_index;
  int _currentIndex;
};
}  // namespace mover