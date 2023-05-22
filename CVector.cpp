#include "CVector.h"
#include "Constant.h"

CVector::CVector() : x_(0.0), y_(0.0) {}

CVector::CVector(float x, float y) : x_(x), y_(y) {}

CVector::CVector(float angle) : x_(cos(angle)), y_(sin(angle)) {}

CVector::CVector(int x, int y) : x_(x), y_(y) {}

CVector::CVector(bool v) : x_(0), y_(0) { is_valid_ = false; }

int CVector::GetDirection() {
  double angle = GetAngle();
  angle = std::fmod(angle + Constant::kPI * 2, Constant::kPI * 2);
  if (Constant::kPI / 4.0 * 5.0 < angle && angle <= Constant::kPI / 4.0 * 7.0)
    return 2;
  if (Constant::kPI / 4.0 * 3.0 < angle && angle <= Constant::kPI / 4.0 * 5.0)
    return 1;
  if (Constant::kPI / 4.0 < angle && angle <= Constant::kPI / 4.0 * 3.0)
    return 0;
  return 3;
}

CVector CVector::GetNorm() const {
  double l = GetLength();
  if (l < Constant::kZeroBorder) return CVector(0.0f, 0.0f);
  return *this / l;
}