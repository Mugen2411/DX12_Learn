#pragma once
#include "CVector.h"

class CSTGInputManager {
 public:
  CSTGInputManager();
  virtual ~CSTGInputManager(){};

  int Up();
  int Down();
  int Right();
  int Left();
  int A();
  int B();
  int C();
  int Slow();
  int Start();
  int Select();

  int LClick(bool isPush);
  int RClick(bool isPush);
  int CClick(bool isPush);

  int MouseX();
  int MouseY();

  CVector GetVector();
  int GetDirection();

  float GetMouseAngle(CVector p);

  virtual int IsChanged() = 0;
  virtual int Update() = 0;

  virtual void SetMouseVisible() = 0;
  virtual void SetMouseInvisible() = 0;

 protected:
  struct ConfigData {
    int up;
    int down;
    int right;
    int left;
    int a;
    int b;
    int c;
    int slow;
    int start;
    int select;
  } config_data_;

  enum StgKeycode {
    kNoConfig = -1,
    kUp,
    kDown,
    kRight,
    kLeft,
    kA,
    kB,
    kC,
    kSlow,
    kStart,
    kSelect
  };

  int pushed_frame_[10];
  int mx_, my_;
  int clicked_frame_[3][2];
  bool is_mouse_shown_;
};
