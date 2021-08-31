#include "Application.hpp"

namespace FloraEngine {

Application *Application::sInstance = nullptr;

Application::Application() {
  sInstance = this;

  mIsRunning        = true;
  mLayerInsertIndex = 0;
  mLayers           = CreateScope<std::vector<ApplicationLayer *>>();

  /* Create Application Window */
  mWindow = CreateScope<Window>();
  mWindow->Init();
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

  /* Attach Layers */
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnAttach();
  }

  /* App Core */
  while (IsRunning()) {

    /* Update each of the application layers */
    for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
      (*mLayers)[idx]->OnUpdate();
    }

    /* Update window, swapping buffers and polling for events.
     *  Exit the application if the window should close.
     *  TODO: Exit Dialogue */
    if (!mWindow->OnUpdate()) {
      mIsRunning = false;
      break;
    }
  }
}

void Application::OnEvent(Event &event) {
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnEvent(event);
  }
}

Application::~Application() {
  /* Detach Layers */
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnDetach();
  }
}

void Application::OnRender() {}

bool Application::IsRunning() {
  return mIsRunning;
}

} // namespace FloraEngine