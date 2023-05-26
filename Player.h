#pragma once
#include "CControllerFactory.h"
#include "CSTGInputManager.h"
#include "DxGraphic.h"
#include "Mover.h"

namespace mover {
class Player : public mover::Mover {
 public:
  Player()
      : _spd(4.0f),
        Mover((Constant::kGameBottomX - Constant::kGameTopX) / 2 +
                  Constant::kGameTopX,
              Constant::kGameBottomY / 4 * 3, 0.0f, 0.0f, 5) {
    _graph = mugen_engine::getIns().LoadGraphic("minoki");
    _marker = mugen_engine::getIns().LoadGraphic("marker");
    _input = CControllerFactory::GetIns().GetController();
  }
  State Update();
  void Render() const;

  void Dead() {}
  void Disappear() {}

  void Dispatch(Mover* m) { m->Hit(this); }

 private:
  float _spd;
  std::weak_ptr<CSTGInputManager> _input;
  std::shared_ptr<mugen_engine::Graphic> _graph;
  std::shared_ptr<mugen_engine::Graphic> _marker;
};
}  // namespace mover