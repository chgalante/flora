#include "Window.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"

namespace FloraEngine {

Window::Window() {

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  glfwInit();
  pWindow = glfwCreateWindow(1280, 720, "FloraEngine", NULL, NULL);
  if (!pWindow) {
    FE_CORE_CRITICAL("GLFW failed to create window");
  }

  glfwMakeContextCurrent(pWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    FE_CORE_CRITICAL("Failed to initialize OpenGL Context!");
  }
  glViewport(0, 0, 640, 480);
}

bool Window::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(pWindow);

  /* Returns false if the window should close to signal app termination */
  return !glfwWindowShouldClose(pWindow);
}

} // namespace FloraEngine