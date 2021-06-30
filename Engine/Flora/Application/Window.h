#pragma once

#include "pch.h"
#include <GLFW/glfw3.h>

namespace FloraEngine {
class Window {
public:
  Window() {

    glfwWindowHint(GLFW_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_VERSION_MINOR, 1);
    GLFWwindow *window = glfwCreateWindow(640, 480, "FloraEngine", NULL, NULL);
  }
};
} // namespace FloraEngine