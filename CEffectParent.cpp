#include "CEffectParent.h"

std::vector<std::shared_ptr<CEffect>> CEffectParent::effectlist_;

void CEffectParent::RegisterEffect(std::shared_ptr<CEffect> e) {
  effectlist_.push_back(e);
}

void CEffectParent::Reset() { effectlist_.clear(); }

bool CEffectParent::empty() { return effectlist_.empty(); }

void CEffectParent::Update() {
  for (std::shared_ptr<CEffect> v : CEffectParent::effectlist_) {
    v->Update();
  }
  auto new_end =
      std::remove_if(effectlist_.begin(), effectlist_.end(),
                     [](std::shared_ptr<CEffect> v) -> bool { return !(*v); });
  effectlist_.erase(new_end, effectlist_.end());
}

void CEffectParent::Render() {
  for (std::shared_ptr<CEffect> v : CEffectParent::effectlist_) {
    v->Render();
  }
}