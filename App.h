#pragma once
#include <Windows.h>
#include <tchar.h>
#include "Engine.h"
#include "DxGraphic.h"

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
  std::shared_ptr<mugen_engine::Graphic> _graph;
};