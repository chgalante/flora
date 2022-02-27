#pragma once
#include "Flora/Vulkan/VulkanDevice.hpp"
#include "Flora/Vulkan/VulkanFramebuffers.hpp"
#include "Flora/Vulkan/VulkanGraphicsPipeline.hpp"
#include "Flora/Vulkan/VulkanImageViews.hpp"
#include "Flora/Vulkan/VulkanInstance.hpp"
#include "Flora/Vulkan/VulkanSwapChain.hpp"
#include "Flora/Vulkan/VulkanWindow.hpp"
#include "pch.h"

namespace FloraEngine {

class VulkanContext {
public:
  VulkanContext();
  ~VulkanContext();

  void Init();

  VulkanWindow *GetWindow();

private:
  Scope<VulkanWindow>           mVulkanWindow;
  Scope<VulkanInstance>         mVulkanInstance;
  Scope<VulkanDevice>           mVulkanDevice;
  Scope<VulkanSwapChain>        mVulkanSwapChain;
  Scope<VulkanImageViews>       mVulkanImageViews;
  Scope<VulkanGraphicsPipeline> mVulkanGraphicsPipeline;
  Scope<VulkanFramebuffers>     mVulkanFramebuffers;
};
} // namespace FloraEngine