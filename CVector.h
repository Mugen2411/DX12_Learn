#pragma once
#include <cmath>

#include "Constant.h"

class CVector {
 public:
  CVector();
  CVector(float, float);
  CVector(float);
  CVector(int, int);
  CVector(bool);

  float x_, y_;
  bool is_valid_ = true;  // true:—LŒø, false:–³Œø

  inline CVector operator+(CVector const v) const {
    return CVector(this->x_ + v.x_, this->y_ + v.y_);
  }
  inline CVector operator-(CVector const v) const {
    return CVector(this->x_ - v.x_, this->y_ - v.y_);
  }
  inline CVector operator+=(CVector const v) {
    this->x_ += v.x_;
    this->y_ += v.y_;
    return *this;
  }
  inline CVector operator-=(CVector const v) {
    this->x_ -= v.x_;
    this->y_ -= v.y_;
    return *this;
  }
  inline CVector operator-() const { return CVector(-x_, -y_); }
  inline CVector operator*(float const v) const {
    return CVector(this->x_ * v, this->y_ * v);
  }
  inline CVector operator/(float const v) const {
    return CVector(this->x_ / v, this->y_ / v);
  }
  inline CVector operator*=(float const v) {
    *this = *this * v;
    return *this;
  }
  inline bool operator!() const { return !is_valid_; }
  inline bool operator<(CVector v) const {
    if (x_ == v.x_) {
      if (y_ < v.y_)
        return true;
      else
        return false;
    } else {
      if (x_ < v.x_)
        return true;
      else
        return false;
    }
  }
  inline bool operator==(CVector v) const {
    if (abs(x_ - v.x_) < Constant::kZeroBorder &&
        abs(y_ - v.y_) < Constant::kZeroBorder)
      return true;
    return false;
  }

  inline double Dot(CVector const v) const {
    return this->x_ * v.x_ + this->y_ * v.y_;
  }
  inline double Cross(CVector const v) const { return v.x_ * y_ - v.y_ * x_; }
  inline double GetLength2() const { return x_ * x_ + y_ * y_; }
  inline double GetLength() const { return sqrt(GetLength2()); }
  inline float GetAngle() const { return (float)atan2(y_, x_); }
  int GetDirection();
  CVector GetNorm() const;

  inline void Zero() {
    if (GetLength2() < Constant::kZeroBorder) {
      x_ *= 0.0001f;
      y_ *= 0.0001f;
    }
  }
  inline void Floor() {
    x_ = std::floor(x_);
    y_ = std::floor(y_);
  }
};