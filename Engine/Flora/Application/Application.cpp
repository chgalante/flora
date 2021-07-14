#include "Application.hpp"

namespace FloraEngine {

Application::Application() {
  mIsRunning        = true;
  mLayerInsertIndex = 0;
  mLayers           = std::vector<ApplicationLayer *>();
}

void Application::PushLayer(ApplicationLayer *application_layer) {
  mLayers.emplace(mLayers.begin() + mLayerInsertIndex, application_layer);
  mLayerInsertIndex++;
}

void Application::PushOverlay(ApplicationLayer *application_overlay) {
  mLayers.emplace_back(application_overlay);
}

void Application::Run() {

  /* Create Application Window */
  Scope<Window> pWindow = CreateScope<Window>();

  /* Attach each of the application layers */
  for (int32_t idx = mLayers.size() - 1; idx >= 0; idx--) {
    mLayers[idx]->OnAttach();
  }

  /* App Core */
  while (IsRunning()) {
    /* Exit from application core loop when the window should close */
    if (!pWindow->OnUpdate()) {
      mIsRunning = false;
    }

    /* Update each of the application layers */
    for (int32_t idx = mLayers.size() - 1; idx >= 0; idx--) {
      mLayers[idx]->OnUpdate();
    }
  }
}

bool Application::IsRunning() {
  return mIsRunning;
}

} // namespace FloraEngine