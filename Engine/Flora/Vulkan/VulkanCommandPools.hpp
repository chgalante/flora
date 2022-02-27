#pragma once
#include "VulkanDevice.hpp"
#include "VulkanFramebuffers.hpp"
#include "VulkanGraphicsPipeline.hpp"
#include "VulkanSwapChain.hpp"
#include "pch.h"

namespace FloraEngine {
class VulkanCommandPools {
public:
  VulkanCommandPools(VulkanDevice *,
                     VulkanSwapChain *,
                     VulkanGraphicsPipeline *,
                     VulkanFramebuffers *);
  ~VulkanCommandPools();

  void Init();
  void Cleanup();

  void RecordCommandBuffer(VkCommandBuffer command_buffer,
                           uint32_t        image_index);

private:
  VulkanDevice           *pVulkanDeviceHandle;
  VulkanSwapChain        *pVulkanSwapChainHandle;
  VulkanGraphicsPipeline *pVulkanGraphicsPipelineHandle;
  VulkanFramebuffers     *pVulkanFramebuffersHandle;
  VkCommandPool           mCommandPool;
  VkCommandBuffer         mCommandBuffer;
};
} // namespace FloraEngine
