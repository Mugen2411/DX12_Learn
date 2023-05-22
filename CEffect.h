#pragma once
#include "CVector.h"

class CEffect {
 public:
  CEffect(CVector pos);
  virtual ~CEffect(){};
  inline bool operator!() { return status_; }
  virtual void Update() = 0;
  virtual void Render() const = 0;

 protected:
  CVector position_;
  int status_;  // 0:ê∂ë∂ÅA1:è¡ñ≈
};
