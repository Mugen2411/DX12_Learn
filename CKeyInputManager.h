#pragma once
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "CSTGInputManager.h"
#include "Engine.h"

class CKeyInputManager final : public CSTGInputManager {
 public:
  CKeyInputManager()
      : CSTGInputManager(),
        _key_config{DIK_UP,      DIK_DOWN,     DIK_RIGHT, DIK_LEFT,       DIK_Z,       DIK_X,
                    DIK_C,    DIK_LSHIFT,
                    DIK_ESCAPE, DIK_SPACE},
        _changed(0) {
  }
  ~CKeyInputManager();
  int IsChanged();
  int Update();
  void Load();

  void SetMouseVisible() { is_mouse_shown_ = true; }

  void SetMouseInvisible() { is_mouse_shown_ = false; }

 private:
  int _key_config[10];
  BYTE _buf[256];
  int _changed;
  LPDIRECTINPUT8 _interface;
  LPDIRECTINPUTDEVICE8 _device;
};
