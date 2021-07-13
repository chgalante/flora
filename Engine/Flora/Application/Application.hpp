#pragma once

#include "ApplicationLayer.hpp"
#include "Flora/Base.hpp"
#include "Window.hpp"
#include "pch.h"

namespace FloraEngine {

class ApplicationLayerStack {
public:
  ApplicationLayerStack();
  ~ApplicationLayerStack() {}

public:
  void PushLayer(Ref<ApplicationLayer> application_layer);
  void PushOverlay(Ref<ApplicationLayer> application_layer);

private:
  static std::vector<Ref<ApplicationLayer>>::iterator iterTop;
  static std::vector<Ref<ApplicationLayer>>           sApplicationLayerStack;
};

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
