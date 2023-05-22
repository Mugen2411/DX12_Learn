#include "CKeyInputManager.h"

#include "Constant.h"

CKeyInputManager::~CKeyInputManager() { _device->Unacquire(); }

int CKeyInputManager::IsChanged() { return _changed; }

int CKeyInputManager::Update() {
  int r = 0;
  _device->GetDeviceState(256, _buf);
  for (int i = 0; i < 10; i++) {
    if (_buf[_key_config[i]] & 0x80) {
      pushed_frame_[i]++;
      if (kUp <= i && i <= kLeft) r = 1;
    } else
      pushed_frame_[i] = 0;
  }
  _changed = r;

  return r;
}

void CKeyInputManager::Load() {
  DirectInput8Create(GetModuleHandle(NULL), DIRECTINPUT_VERSION,
                     IID_IDirectInput8, (void**)&_interface, nullptr);
  _interface->CreateDevice(GUID_SysKeyboard, &_device, NULL);
  _device->SetDataFormat(&c_dfDIKeyboard);
  _device->SetCooperativeLevel(mugen_engine::getIns().getWindowHandle(),
                               DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
  _device->Acquire();
}
