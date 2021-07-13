#pragma once

#include "ApplicationLayerStack.hpp"
#include "Flora/Base.hpp"
#include "Window.hpp"
#include "pch.h"

namespace FloraEngine {

class Application {
public:
  Application() {
    bIsRunning             = true;
    pApplicationLayerStack = CreateScope<ApplicationLayerStack>();
  }
  ~Application() {}

public:
  void Run();
  bool IsRunning();

public:
private:
  bool                         bIsRunning = false;
  Scope<Window>                pWindow;
  Scope<ApplicationLayerStack> pApplicationLayerStack;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine
