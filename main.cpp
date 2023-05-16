#include "App.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
  App app(1280, 960, _T("Minoki"));
  app.Initialize();
  while (app.Process()) {
    app.Update();
  }
  return 0;
}
