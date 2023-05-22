#pragma once
#include "CControllerFactory.h"
#include "CSTGInputManager.h"
#include "DxGraphic.h"
#include "Mover.h"

namespace mover {
class Player : public mover::Mover {
 public:
  Player()
      : _x((Constant::kGameBottomX - Constant::kGameTopX) / 2 +
           Constant::kGameTopX),
        _y(Constant::kGameBottomY / 4 * 3),
        _sp(4.0f) {
    _graph = mugen_engine::getIns().LoadGraphic("minoki");
    _input = CControllerFactory::GetIns().GetController();
  }
  State Update();
  void Render() const;

  void Dead() {}
  void Disappear() {}

 private:
  float _x, _y;
  float _sp;
  std::weak_ptr<CSTGInputManager> _input;
  std::shared_ptr<mugen_engine::Graphic> _graph;
};
}  // namespace mover