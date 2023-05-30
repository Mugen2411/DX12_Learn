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
  void RegisterBullet(std::shared_ptr<Mover> m,
                      bool isDependOnParent);
  void RegisterBullet(std::shared_ptr<Player> p, bool isDependOnParent) {
    player_.swap(p);
  }
  void Update();
  void Render() const;

 private:
  static Manager _instance;
  Manager();
  std::vector<MOVER_ELEM> bullet_list_;
  std::vector<MOVER_ELEM> enemy_list_;
  std::shared_ptr<Player> player_;
  std::queue<int> unused_bullet_index_;
  std::queue<int> unused_enemy_index;
  std::queue<int> dead_bullet_index_;
  std::queue<int> dead_enemy_index_;
  int currentEnemyIndex_;
};
}  // namespace mover