#pragma once
#include "DxGraphic.h"
#include "Random.h"

class Stage{
 public:
  Stage();
  void Initialize();
  void Update();
  void Render() const;

 protected:
  int _cnt;
  std::shared_ptr<mugen_engine::Graphic> _frame;
  void Dead() {}
  void Disappear() {}
};