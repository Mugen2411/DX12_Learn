#include "CSTGInputManager.h"

CSTGInputManager::CSTGInputManager()
    : pushed_frame_{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      config_data_{0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
      clicked_frame_{{0, 0}, {0, 0}, {0, 0}},
      is_mouse_shown_(true),
      mx_(320),
      my_(240) {}

int CSTGInputManager::Up() { return pushed_frame_[kUp]; }

int CSTGInputManager::Down() { return pushed_frame_[kDown]; }

int CSTGInputManager::Right() { return pushed_frame_[kRight]; }

int CSTGInputManager::Left() { return pushed_frame_[kLeft]; }

int CSTGInputManager::A() { return pushed_frame_[kA]; }

int CSTGInputManager::B() { return pushed_frame_[kB]; }

int CSTGInputManager::C() { return pushed_frame_[kC]; }

int CSTGInputManager::Slow() { return pushed_frame_[kSlow]; }

int CSTGInputManager::Start() { return pushed_frame_[kStart]; }

int CSTGInputManager::Select() { return pushed_frame_[kSelect]; }

int CSTGInputManager::LClick(bool isPush) { return clicked_frame_[0][isPush]; }

int CSTGInputManager::RClick(bool isPush) { return clicked_frame_[1][isPush]; }

int CSTGInputManager::CClick(bool isPush) { return clicked_frame_[2][isPush]; }

int CSTGInputManager::MouseX() { return mx_; }

int CSTGInputManager::MouseY() { return my_; }

CVector CSTGInputManager::GetVector() {
  CVector tmp(0.0f, 0.0f);
  if (pushed_frame_[kUp] != 0) tmp -= CVector(0.0f, 1.0f);
  if (pushed_frame_[kDown] != 0) tmp += CVector(0.0f, 1.0f);
  if (pushed_frame_[kRight] != 0) tmp += CVector(1.0f, 0.0f);
  if (pushed_frame_[kLeft] != 0) tmp -= CVector(1.0f, 0.0f);
  return tmp.GetNorm();
}

int CSTGInputManager::GetDirection() {
  if (pushed_frame_[kUp] > 0) {
    if (pushed_frame_[kRight] > pushed_frame_[kUp])
      return kRight;
    else
      return kUp;
    if (pushed_frame_[kLeft] > pushed_frame_[kUp])
      return kLeft;
    else
      return kUp;
  }
  if (pushed_frame_[kDown] > 0) {
    if (pushed_frame_[kRight] > pushed_frame_[kDown])
      return kRight;
    else
      return kDown;
    if (pushed_frame_[kLeft] > pushed_frame_[kDown])
      return kLeft;
    else
      return kDown;
  }
  if (pushed_frame_[kRight] > 0) {
    if (pushed_frame_[kUp] > pushed_frame_[kRight])
      return kUp;
    else
      return kRight;
    if (pushed_frame_[kDown] > pushed_frame_[kRight])
      return kDown;
    else
      return kRight;
  }
  if (pushed_frame_[kLeft] > 0) {
    if (pushed_frame_[kUp] > pushed_frame_[kLeft])
      return kUp;
    else
      return kLeft;
    if (pushed_frame_[kDown] > pushed_frame_[kLeft])
      return kDown;
    else
      return kLeft;
  }
  return kNoConfig;
}

float CSTGInputManager::GetMouseAngle(CVector p) {
  return (float)atan2(my_ - p.y_, mx_ - p.x_);
}
