#pragma once
#include <Windows.h>
#include <tchar.h>
#include "Engine.h"
#include "DxGraphic.h"
#include "Stage.h"
#include "Fps.h"

class App {
 public:
  App(int width, int height, LPCWSTR windowTitle);
  ~App();
  void Initialize();
  bool Process();
  void Update();
 private:
  int _window_width, _window_height;
  LPCWSTR _windowTitle;
  HWND _hwnd;
  Stage _stage;
  Fps _fps;
};