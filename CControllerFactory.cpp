#include "CControllerFactory.h"

CControllerFactory::CControllerFactory() {
  auto tmp = std::make_shared<CKeyInputManager>();
  tmp->Load();
  current_ = tmp;
}

std::weak_ptr<CSTGInputManager> CControllerFactory::GetController() {
  return current_;
}

void CControllerFactory::Update() { current_->Update(); }
