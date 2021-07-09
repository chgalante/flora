#include "Flora/Application/Window.h"

namespace FloraEngine {

Window::Window() {

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  mWindow = glfwCreateWindow(640, 480, "FloraEngine", NULL, NULL);
  glfwMakeContextCurrent(mWindow);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize OpenGL context" << std::endl;
  }
  glViewport(0, 0, 640, 480);
}

void Window::OnUpdate() {
  glfwPollEvents();
  glfwSwapBuffers(mWindow);
}

} // namespace FloraEngine