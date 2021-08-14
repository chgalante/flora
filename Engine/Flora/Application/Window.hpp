#pragma once

#include "pch.h"

// GLAD
#include <glad/glad.h>
// GLFW
#include <GLFW/glfw3.h>

namespace FloraEngine {

class Window {

public:
  Window();
  ~Window(){};

  bool OnUpdate();

private:
  GLFWwindow *pWindow;
};

} // namespace FloraEngine