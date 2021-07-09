#pragma once

#include "Flora/Application/Window.h"
#include "Flora/Base.h"

namespace FloraEngine {

class Application {
public:
  Application() {
    bIsRunning = true;
  }
  ~Application() {}

public:
  void Run();
  bool IsRunning();

public:
  virtual void OnAttach() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnDetach() = 0;

private:
  bool          bIsRunning = false;
  Scope<Window> pWindow;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine
