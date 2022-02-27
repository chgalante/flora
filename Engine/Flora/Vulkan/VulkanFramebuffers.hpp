#pragma once
#include "VulkanDevice.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanImageViews.hpp"
#include "VulkanSwapChain.hpp"
#include "pch.h"

namespace FloraEngine {
class VulkanFramebuffers {
public:
  VulkanFramebuffers(VulkanDevice *,
                     VulkanSwapChain *,
                     VulkanImageViews *,
                     VulkanGraphicsPipeline *);
  ~VulkanFramebuffers();

  void Init();
  void Cleanup();

private:
  VulkanDevice           *pVulkanDeviceHandle;
  VulkanSwapChain        *pVulkanSwapChainHandle;
  VulkanImageViews       *pVulkanImageViewsHandle;
  VulkanGraphicsPipeline *pVulkanGraphicsPipelineHandle;

  std::vector<VkFramebuffer> mFramebuffers;
};
} // namespace FloraEngine
