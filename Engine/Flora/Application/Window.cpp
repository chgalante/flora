#include "Window.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"

namespace FloraEngine {

Window::Window() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  mGLFWWindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

Window::~Window() {
  glfwDestroyWindow(mGLFWWindow);
  glfwTerminate();
}

bool Window::OnUpdate() {
  glfwPollEvents();

  /* Returns false if the window should close to signal app termination */
  return !glfwWindowShouldClose(mGLFWWindow);
}

} // namespace FloraEngine