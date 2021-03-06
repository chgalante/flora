#include "Application.hpp"

namespace FloraEngine {

Application::Application() {

  mLayers = CreateScope<std::vector<ApplicationLayer *>>();

  mIsRunning        = true;
  mLayerInsertIndex = 0;
}

/* ApplicationLayer Stack:
 * ----------------------------------------------
 * Overlay3     <- insert overlay   vector back
 * Overlay2                               |
 * Overlay1                               | dir
 * Layer3       <- insert layer           |
 * Layer2                                 V
 * Layer1                           vector front
 * ----------------------------------------------
 *
 * Overlays and layers are processed for updates
 * from the back to the front of the underlying
 * vector container. Overlays are placed on top
 * of the stack, layers are placed after the
 * last overlay.
 */

void Application::PushLayer(ApplicationLayer *application_layer) {

  /* Push layer on layer stack */
  mLayers->emplace(mLayers->begin() + mLayerInsertIndex, application_layer);
  mLayerInsertIndex++;
}

void Application::PushOverlay(ApplicationLayer *application_overlay) {

  /* Push overlay on top of overlay stack */
  mLayers->emplace_back(application_overlay);
}

void Application::Run() {

  /* Create Application Window */
  Scope<Window> pWindow = CreateScope<Window>();

  /* Attach each of the application layers */
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnAttach();
  }

  /* App Core */
  while (IsRunning()) {
    /* Exit from application core loop when the window should close */
    if (!pWindow->OnUpdate()) {
      mIsRunning = false;
      break;
    }

    /* Update each of the application layers */
    for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
      (*mLayers)[idx]->OnUpdate();
    }
  }
}

bool Application::IsRunning() {
  return mIsRunning;
}

} // namespace FloraEngine