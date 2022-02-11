#pragma once
#include "pch.h"

namespace FloraEngine {

class VulkanWindow {

public:
  VulkanWindow();
  ~VulkanWindow();

  bool OnUpdate();

  inline GLFWwindow *GetGLFWHandle() {
    return mGLFWwindow;
  }

  inline const uint32_t GetWidth() {
    return WIDTH;
  }

  inline const uint32_t GetHeight() {
    return HEIGHT;
  }

private:
  GLFWwindow    *mGLFWwindow;
  const uint32_t WIDTH  = 800;
  const uint32_t HEIGHT = 600;
};

} // namespace FloraEngine