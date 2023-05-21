#pragma once
#include <functional>
#include <list>
#include <memory>

namespace Composite {

class Entry {
 public:
  enum class State { kAlive, kDead, kDisappear };
  Entry() : _state(State::kAlive) {}

  State BaseUpdate();
  void BaseRender() const;

 protected:
  State _state;
  void addChild(std::shared_ptr<Entry> c);

  virtual State Update() = 0;
  virtual void Render() const = 0;
  virtual void Dead() = 0;
  virtual void Disappear() = 0;

 private:
  std::list<std::shared_ptr<Entry>> _childs;
};

}  // namespace Composite