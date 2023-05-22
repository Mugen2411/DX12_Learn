#include "BulletBase.h"

#include "Constant.h"

namespace bullet {
BulletBase::BulletBase(std::string gid, int gnum, float x, float y, float a,
                       float sp)
    : Mover(),
      _graph(mugen_engine::getIns().LoadGraphic(gid)),
      _x(x),
      _y(y),
      _a(a),
      _sp(sp),
      _gnum(gnum) {}
BulletBase::State BulletBase::Update() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  if (_x < 0 || _x > Constant::kScreenWidth || _y < 0 ||
      _y > Constant::kScreenHeight)
    _state = State::kDead;
  return _state;
}
void BulletBase::Render() const {
  _graph->Draw(_x, _y, _a, 1.0f, Constant::Priority::kBullet, _gnum, 1);
}
}  // namespace bullet
