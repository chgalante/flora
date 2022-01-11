#include "Application.hpp"

namespace FloraEngine {

Application::Application() {
  /* Initialize Members */
  mLayers           = CreateScope<std::vector<ApplicationLayer *>>();
  mIsRunning        = true;
  mLayerInsertIndex = 0;
}

#ifdef FE_DEBUG
#endif

void Application::Run() {

  /* Init Application Window */
  mWindow = CreateScope<Window>();

  /* Init Graphics Context */
  mGraphicsContext = CreateScope<GraphicsContext>();
  mGraphicsContext->Init();

  /* Attach each of the application layers */
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnAttach();
  }

  /* App Core */
  while (IsRunning()) {
    /* Exit from application core loop when the window should close */
    if (!mWindow->OnUpdate()) {
      mIsRunning = false;
      break;
    }

    /* Update each of the application layers */
    for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
      (*mLayers)[idx]->OnUpdate();
    }
  }
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

bool Application::IsRunning() {
  return mIsRunning;
}

Application::~Application() {}
} // namespace FloraEngine