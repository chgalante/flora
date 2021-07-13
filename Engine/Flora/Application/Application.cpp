#include "Application.hpp"

namespace FloraEngine {

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