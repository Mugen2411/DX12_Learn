#include "Composit.h"

namespace Composite {

Entry::State Entry::BaseUpdate() {
  for (auto itr = _childs.begin(); itr != _childs.end();) {
    (*itr)->BaseUpdate();
    if ((*itr)->_state == State::kDead) {
      (*itr)->Dead();
      itr = _childs.erase(itr);
      continue;
    }
    if ((*itr)->_state == State::kDisappear) {
      (*itr)->Disappear();
      itr = _childs.erase(itr);
      continue;
    }
    itr++;
  }
  return Update();
}

void Entry::BaseRender() const {
  for (auto &i : _childs) {
    i->BaseRender();
  }
  Render();
}

void Entry::addChild(std::shared_ptr<Entry> c) { _childs.push_back(c); }

}  // namespace Composite