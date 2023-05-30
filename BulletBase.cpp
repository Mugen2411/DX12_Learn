#include "BulletBase.h"

#include "Constant.h"

namespace bullet {
BulletBase::BulletBase(Descriptor::BASE_DESC &desc)
    : Mover(desc.x, desc.y, desc.angle, desc.speed,
            bullet_type_data[static_cast<int>(desc.type)].radius),
      _graph(mugen_engine::getIns().LoadGraphic(
          bullet_type_data[static_cast<int>(desc.type)].gid)),
      _gnum(bullet_type_data[static_cast<int>(desc.type)].offset +
            static_cast<int>(desc.color)),
      _blend_type(desc.blendtype),
      _type(desc.type),
      _color(desc.color),
      _cnt(0) {
  CEffectParent::RegisterEffect(std::make_shared<CEffect_BulletBorn>(
      CVector(_x, _y), CVector(_a) * _sp,
      bullet_type_data[static_cast<int>(_type)].radius * 12,
      static_cast<int>(_color)));
}
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
  return std::make_shared<BulletBase>(desc);
}
}  // namespace bullet
