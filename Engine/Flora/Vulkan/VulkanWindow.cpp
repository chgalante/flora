#include "VulkanWindow.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"

namespace FloraEngine {

VulkanWindow::VulkanWindow() {
  glfwInit();

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

  mGLFWwindow = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
}

VulkanWindow::~VulkanWindow() {
  glfwDestroyWindow(mGLFWwindow);
  glfwTerminate();
}

bool VulkanWindow::OnUpdate() {
  glfwPollEvents();

  /* Returns false if the window should close to signal app termination */
  return !glfwWindowShouldClose(mGLFWwindow);
}

} // namespace FloraEngine