#include "Player.h"
namespace mover {
Mover::State Player::Update() {
  CVector v = _input.lock()->GetVector() * _sp;
  _x += v.x_;
  _y += v.y_;
  return _state;
}

void Player::Render() const {
  _graph->Draw(_x, _y, 0.0f, 1.0f, Constant::Priority::kPlayer, 0);
  _marker->Draw(_x, _y, 0.0f, 1.0f, Constant::Priority::kMarker, 0);
}
}  // namespace mover