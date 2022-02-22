#pragma once
#include "Flora/Vulkan/VulkanSwapChain.hpp"
#include "pch.h"

namespace FloraEngine {
class VulkanImageViews {
public:
  VulkanImageViews(VulkanSwapChain *, VulkanDevice *);
  ~VulkanImageViews();

  void Init();
  void Cleanup();

private:
  void create_vulkan_image_views();

private:
  VulkanSwapChain         *pVulkanSwapChainHandle;
  VulkanDevice            *pVulkanDeviceHandle;
  std::vector<VkImageView> mSwapChainImageViews;
};

} // namespace FloraEngine
