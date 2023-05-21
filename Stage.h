#pragma once

class Stage{
 public:
  Stage();
  void Update();
  void Render() const;

 protected:
  int _cnt;
  void Dead() {}
  void Disappear() {}
};