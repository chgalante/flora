#pragma once

#include "ApplicationLayer.hpp"
#include "Flora/Base.hpp"
#include "Window.hpp"
#include "pch.h"

namespace FloraEngine {

class Application {

public:
  Application();
  ~Application() {}

public:
  void Run();
  bool IsRunning();

  void PushLayer(ApplicationLayer *application_layer);
  void PushOverlay(ApplicationLayer *application_overlay);

private:
  bool                            mIsRunning = false;
  Scope<Window>                   mWindow;
  std::vector<ApplicationLayer *> mLayers;
  uint32_t                        mLayerInsertIndex;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine
