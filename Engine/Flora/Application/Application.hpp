#pragma once

#include "ApplicationLayer.hpp"
#include "Flora/Base.hpp"
#include "Flora/Renderer/RendererContext.hpp"
#include "Window.hpp"
#include "pch.h"

namespace FloraEngine {

class Application {

public:
  Application();
  ~Application();

  void Run();
  bool IsRunning();

  void PushLayer(ApplicationLayer *application_layer);
  void PushOverlay(ApplicationLayer *application_overlay);

  virtual void OnAttach() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnDetach() = 0;

private:
  Scope<std::vector<ApplicationLayer *>> mLayers;
  Scope<Window>                          mWindow;
  Scope<RendererContext>                 mRendererContext;
  bool                                   mIsRunning = false;
  uint32_t                               mLayerInsertIndex;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine
