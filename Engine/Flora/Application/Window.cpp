#include "Flora/Application/Window.hpp"
#include "Event/InputEvent.hpp"
#include "Event/WindowEvent.hpp"
#include "Flora/Application/Application.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"

namespace FloraEngine {

bool Window::Init() {

  int nbr_attribs, version_major, version_minor;

  /* Initialize GLFW and Create a Window */
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  GLenum err = glfwInit();
  mWindow    = glfwCreateWindow(1280, 720, "FloraEngine", NULL, NULL);
  if (!mWindow) {
    FE_CORE_CRITICAL("GLFW failed to create window");
  }
  glfwMakeContextCurrent(mWindow);

  /* Initialize OpenGL Context*/
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    FE_CORE_CRITICAL("Failed to initialize OpenGL Context!");
  }
  glViewport(0, 0, 640, 480);

  /* Log OpenGL context info */
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nbr_attribs);
  glGetIntegerv(GL_MAJOR_VERSION, &version_major);
  glGetIntegerv(GL_MINOR_VERSION, &version_minor);
  FE_CORE_TRACE("Initialized OpenGL Context version {0}.{1}",
                version_major,
                version_minor);
  FE_CORE_TRACE("Maximum number of vertex attributes: {0}", nbr_attribs);

  /* Route events to the application */

  /* WindowClosedEvent */
  glfwSetWindowCloseCallback(mWindow, [](GLFWwindow *window) {
    auto event = CreateScope<WindowClosedEvent>();
    Application::Get().OnEvent(*event);
  });

  /* WindowResizedEvent */
  glfwSetWindowSizeCallback(
      mWindow,
      [](GLFWwindow *window, int width, int height) {
        auto event = CreateScope<WindowResizedEvent>(width, height);
        Application::Get().OnEvent(*event);
      });

  /* WindowMovedEvent */
  glfwSetWindowPosCallback(mWindow, [](GLFWwindow *window, int xpos, int ypos) {
    auto event = CreateScope<WindowMovedEvent>(xpos, ypos);
    Application::Get().OnEvent(*event);
  });

  /* WindowLostFocusEvent */
  glfwSetWindowFocusCallback(mWindow, [](GLFWwindow *window, int focused) {
    if (focused) {
      /* Do Nothing */
    } else {
      auto event = CreateScope<WindowLostFocusEvent>();
      Application::Get().OnEvent(*event);
    }
  });

  /* KeyPressedEvent & KeyReleasedEvent */
  glfwSetKeyCallback(
      mWindow,
      [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        if (GLFW_PRESS == action || GLFW_REPEAT == action) {
          auto event = CreateScope<KeyPressedEvent>(key, scancode, mods);
          Application::Get().OnEvent(*event);
        } else if (GLFW_RELEASE == action) {
          auto event = CreateScope<KeyReleasedEvent>(key, scancode, mods);
          Application::Get().OnEvent(*event);
        }
      });

  /* MouseButtonPressedEvent & MouseButtonReleasedEvent*/
  glfwSetMouseButtonCallback(
      mWindow,
      [](GLFWwindow *window, int button, int action, int mods) {
        if (GLFW_PRESS == action) {
          auto event = CreateScope<MouseButtonPressedEvent>(button, mods);
          Application::Get().OnEvent(*event);
        } else if (GLFW_RELEASE == action) {
          auto event = CreateScope<MouseButtonReleasedEvent>(button, mods);
          Application::Get().OnEvent(*event);
        }
      });

  /* MouseMovedEvent */
  glfwSetCursorPosCallback(mWindow,
                           [](GLFWwindow *window, double xpos, double ypos) {
                             auto event =
                                 CreateScope<MouseMovedEvent>(xpos, ypos);
                             Application::Get().OnEvent(*event);
                           });

  return true;
}

bool Window::OnUpdate() {
  glfwSwapBuffers(mWindow);
  glfwPollEvents();

  /* Returns false if the window should close to signal app termination */
  return !glfwWindowShouldClose(mWindow);
}

} // namespace FloraEngine