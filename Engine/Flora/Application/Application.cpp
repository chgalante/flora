#include "Application.h"

namespace FloraEngine {

void Application::Run() {
  /* Create Application Window */
  Scope<Window> pWindow = CreateScope<Window>();

  /* Attach Application */
  OnAttach();

  /* App Core */
  while (IsRunning()) {
    // OnUpdate();
    pWindow->OnUpdate();
  }
}

bool Application::IsRunning() {
  return bIsRunning;
}

} // namespace FloraEngine