#pragma once
#include <memory>

#include "CKeyInputManager.h"
#include "Singleton.h"

class CControllerFactory : public Singleton<CControllerFactory> {
 public:
  CControllerFactory();
  std::weak_ptr<CSTGInputManager> GetController();
  void Update();

  protected:
  std::shared_ptr<CSTGInputManager> current_;
};
