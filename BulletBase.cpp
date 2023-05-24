#include "BulletBase.h"

#include "Constant.h"

namespace bullet {
BulletBase::BulletBase(Type type, Color color, float x, float y, float a,
                       float sp, BlendType blendtype)
    : Mover(x, y, a, sp, bullet_type_data[static_cast<int>(type)].radius),
      _graph(mugen_engine::getIns().LoadGraphic(
          bullet_type_data[static_cast<int>(type)].gid)),
      _gnum(bullet_type_data[static_cast<int>(type)].offset +
            static_cast<int>(color)),
      _blend_type(blendtype),
      _type(type),
      _color(color),
      _cnt(0) {}
void BulletBase::Process() {
  _x += cos(_a) * _sp;
  _y += sin(_a) * _sp;
  _cnt++;
}
void BulletBase::OutJudge() {
  if (_x < -abs(_radius) * 2 ||
      _x > Constant::kScreenWidth + abs(_radius) * 2 ||
      _y < -abs(_radius) * 2 || _y > Constant::kScreenHeight + abs(_radius) * 2)
    _state = State::kDisappear;
}
BulletBase::State BulletBase::Update() {
  Process();
  OutJudge();
  return _state;
}
void BulletBase::Render() const {
  _graph->Draw(_x, _y, _a, 1.0f, Constant::Priority::kBullet, _gnum,
               static_cast<int>(_blend_type));
}
std::shared_ptr<BulletBase> Create(Descriptor::BASE_DESC desc) {
  return std::make_shared<BulletBase>(desc.type, desc.color, desc.x, desc.y,
                                      desc.angle, desc.speed, desc.blendtype);
}
}  // namespace bullet
