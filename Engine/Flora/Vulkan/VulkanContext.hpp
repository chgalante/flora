#pragma once
#include "Flora/Application/Window.hpp"
#include "Flora/Vulkan/VulkanDevice.hpp"
#include "Flora/Vulkan/VulkanInstance.hpp"
#include "pch.h"

namespace FloraEngine {

class VulkanContext {
public:
  VulkanContext(Window *);
  ~VulkanContext();

  void Init();

private:
  Window *pWindow;

  Scope<VulkanInstance> mVulkanInstance;
  Scope<VulkanDevice>   mVulkanDevice;
};
} // namespace FloraEngine