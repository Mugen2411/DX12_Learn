#pragma once

class EnemyMoverInterface {
 public:
  EnemyMoverInterface(): cnt_(0) {}
  virtual ~EnemyMoverInterface() {}

  virtual void Update() = 0;

 protected:
  int cnt_;
};
