#include "App.h"

#include "DxGraphic.h"
#include "CControllerFactory.h"
#include "CEffectParent.h"

LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
  if (msg == WM_DESTROY) {
    PostQuitMessage(0);
    return 0;
  }
  return DefWindowProc(hwnd, msg, wparam, lparam);
}

WNDCLASSEX windowClass;

App::App(int width, int height, LPCWSTR windowTitle)
    : _window_width(width), _window_height(height), _windowTitle(windowTitle) {
  windowClass.cbSize = sizeof(WNDCLASSEX);
  windowClass.lpfnWndProc = (WNDPROC)WinProc;
  windowClass.lpszClassName = _T("MugenDX12");
  windowClass.hInstance = GetModuleHandle(nullptr);

  RegisterClassEx(&windowClass);
  RECT wrc = {0, 0, _window_width, _window_height};
  AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

  _hwnd = CreateWindow(windowClass.lpszClassName, _windowTitle,
                       WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX,
                       CW_USEDEFAULT, CW_USEDEFAULT,
                       wrc.right - wrc.left, wrc.bottom - wrc.top, nullptr,
                       nullptr, windowClass.hInstance, nullptr);

  DWORD dw = GetLastError();

  ShowWindow(_hwnd, SW_SHOW);
}

App::~App() {
  UnregisterClass(windowClass.lpszClassName, windowClass.hInstance);
}

void App::Initialize() {
  mugen_engine::getIns().Initialize(1280, 960, _hwnd);
  //mugen_engine::getIns().LoadGraphic("1", _T("M.png"));
  mugen_engine::getIns().LoadGraphic(
      "B_small", _T("media/graphic/bullet/small.png"), 32, 32, 32, 8, 4);
  mugen_engine::getIns().LoadGraphic(
      "eff_bulletdelete", _T("media/graphic/system/bullet_delete.png"), 32, 32, 8, 8, 1);
}

bool App::Process() {
  MSG msg = {};
  if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  if (msg.message == WM_QUIT) {
    return false;
  }
  mugen_engine::getIns().Process();
  CControllerFactory::GetIns();
  return true;
}

void App::Update() {
  _stage.Update();
  CEffectParent::Update();
  _stage.Render();
  CEffectParent::Render();
  mugen_engine::getIns().ScreenFlip();
  _fps.Update();
  _fps.Wait();
}
