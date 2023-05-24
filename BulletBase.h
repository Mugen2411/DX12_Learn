#pragma once
#include <memory>
#include <string>

#include "CEffectParent.h"
#include "CEffect_BulletDelete.h"
#include "DxGraphic.h"
#include "Engine.h"
#include "Mover.h"

namespace bullet {
enum class Type { kSmall, kRice, kWedge, kRing, kArrow };
enum class Color {
  kRed,
  kOrange,
  kYellow,
  kGreen,
  kCyan,
  kBlue,
  kPurple,
  kGray
};
enum class BlendType {
    kNone, kAdd, kAlpha
};
struct BulletTypeData {
  std::string gid;
  int offset;
  int radius;
};

static std::vector<BulletTypeData> bullet_type_data{{"B_small", 0, 12},
                                     {"B_small", 8, 8},
                                     {"B_small", 16, 8},
                                     {"B_small", 24, 10},
                                     {"B_small", 32, 8}};
namespace Descriptor {
struct BASE_DESC {
  float x, y;
  float angle, speed;
  Type type;
  Color color;
  BlendType blendtype = BlendType::kNone;
};
}  // namespace Descriptor
class BulletBase : public mover::Mover {
 public:
  BulletBase(Type type, Color color, float x, float y, float a, float sp,
             BlendType blendtype);

 protected:
  float _x, _y;
  float _a, _sp;
  std::shared_ptr<mugen_engine::Graphic> _graph;
  int _gnum;
  BlendType _blend_type;
  Type _type;
  Color _color;

  int _cnt;

  void Process();
  void OutJudge();
  virtual State Update();
  virtual void Render() const;

  void Dead() {
    CEffectParent::RegisterEffect(std::make_shared<CEffect_BulletDelete>(
        CVector(_x, _y), CVector(_a) * _sp, 32, static_cast<int>(_color)));
  };
  void Disappear(){
    CEffectParent::RegisterEffect(std::make_shared<CEffect_BulletDelete>(
        CVector(_x, _y), CVector(_a) * _sp, 32, static_cast<int>(_color)));
  };
};

std::shared_ptr<BulletBase> Create(Descriptor::BASE_DESC desc);
}  // namespace bullet