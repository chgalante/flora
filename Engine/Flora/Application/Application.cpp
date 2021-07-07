#include "Application.h"

namespace FloraEngine {

void Application::Run() {
  std::cout << "FloraEngine Started" << std::endl;
  while (IsRunning()) {
    OnUpdate();
  }
}

bool Application::IsRunning() {
  return bIsRunning;
}

} // namespace FloraEngine