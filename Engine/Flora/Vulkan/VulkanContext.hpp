#pragma once
#include "Flora/Vulkan/VulkanDevice.hpp"
#include "Flora/Vulkan/VulkanInstance.hpp"
#include "Flora/Vulkan/VulkanSwapChain.hpp"
#include "Flora/Vulkan/VulkanWindow.hpp"
#include "pch.h"

namespace FloraEngine {

class VulkanContext {
public:
  VulkanContext(VulkanWindow *);
  ~VulkanContext();

  void Init();

private:
  VulkanWindow *pWindow;

  Scope<VulkanInstance>  mVulkanInstance;
  Scope<VulkanDevice>    mVulkanDevice;
  Scope<VulkanSwapChain> mVulkanSwapChain;
};
} // namespace FloraEngine