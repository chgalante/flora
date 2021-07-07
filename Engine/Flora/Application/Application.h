#pragma once

#include "pch.h"

namespace FloraEngine {

class Application {
public:
  Application() {
    bIsRunning = true;
    OnAttach();
  }

  ~Application() {}

  void         Run();
  bool         IsRunning();
  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();

private:
  bool bIsRunning = false;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine