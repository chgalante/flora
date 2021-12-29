#pragma once

#include "pch.h"

namespace FloraEngine {

class Window {

public:
  Window();
  ~Window();

  bool OnUpdate();

private:
  GLFWwindow *   mGLFWWindow;
  const uint32_t WIDTH  = 800;
  const uint32_t HEIGHT = 600;
};

} // namespace FloraEngine