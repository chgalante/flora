#include "Application.hpp"

namespace FloraEngine {

ApplicationLayerStack::ApplicationLayerStack() {
  iterTop = sApplicationLayerStack.begin();
}

void ApplicationLayerStack::PushLayer(Ref<ApplicationLayer> application_layer) {
  sApplicationLayerStack.insert(iterTop, application_layer);
}
void ApplicationLayerStack::PushOverlay(
    Ref<ApplicationLayer> application_layer) {
  sApplicationLayerStack.insert(iterTop + 1, application_layer);
}

void Application::Run() {
  /* Create Application Window */
  Scope<Window> pWindow = CreateScope<Window>();

  /* App Core */
  while (IsRunning()) {
    /* Exit from application core loop when the window should close */
    if (!pWindow->OnUpdate()) {
      bIsRunning = false;
    }
  }
}

bool Application::IsRunning() {
  return bIsRunning;
}

} // namespace FloraEngine