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

class List {
 public:
  List(const int maxIndex) : list_(maxIndex), kMaxIndex(maxIndex) {
    for (int i = 0; i < kMaxIndex; i++) {
      unused_index_.push(i);
    }
  }
  void Register(std::shared_ptr<Mover> m, int parent);
  void Update();
  void Render() const;
  void Dispatch(List rhs);
  void Dispatch(std::shared_ptr<Mover> m);
  int GetCurrentIndex() { return currentIndex_; }
 private:
  const int kMaxIndex;
  int currentIndex_ = -1;
  std::vector<MOVER_ELEM> list_;
  std::queue<int> unused_index_;
  std::queue<int> dead_index_;
};

class Manager {
 public:
  static Manager& getIns() { return _instance; }
  const int kMaxIndex;
  void RegisterBullet(std::shared_ptr<Mover> m, bool isDependOnParent);
  void RegisterEnemy(std::shared_ptr<Mover> m, bool isDependOnParent);
  void RegisterPlayer(std::shared_ptr<Player> p, bool isDependOnParent) {
    player_.swap(p);
  }
  void Update();
  void Render() const;

 private:
  static Manager _instance;
  Manager();
  List bullet_list_;
  List enemy_list_;
  std::shared_ptr<Player> player_;
};
}  // namespace mover