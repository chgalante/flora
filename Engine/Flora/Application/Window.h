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

public:
  void OnUpdate();

private:
  GLFWwindow *mWindow;
};
} // namespace FloraEngine