#pragma once
#include "Flora/Application/ApplicationLayer.hpp"
#include "Flora/Application/Event/Event.hpp"
#include "Flora/Application/Window.hpp"
#include "Flora/Base.hpp"
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

  void OnEvent(Event &e);
  void OnRender();

  static Application &Get() {
    return *sInstance;
  }

  virtual void OnAttach() = 0;
  virtual void OnUpdate() = 0;
  virtual void OnDetach() = 0;

private:
  static Application *                   sInstance;
  Scope<std::vector<ApplicationLayer *>> mLayers;
  Scope<Window>                          mWindow;

  bool     mIsRunning = false;
  uint32_t mLayerInsertIndex;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine
