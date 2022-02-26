#pragma once
#include "VulkanDevice.hpp"
#include "VulkanSwapChain.hpp"
#include "pch.h"

namespace FloraEngine {
class VulkanGraphicsPipeline {
public:
  VulkanGraphicsPipeline(VulkanDevice *, VulkanSwapChain *);
  ~VulkanGraphicsPipeline();

  void Init();
  void Cleanup();

private:
  void compile_glsl_to_spirv();

private:
  VulkanDevice    *pVulkanDeviceHandle;
  VulkanSwapChain *pVulkanSwapChainHandle;

  VkShaderModule   mVertexShaderModule;
  VkShaderModule   mFragmentShaderModule;
  VkViewport       mViewport;
  VkRect2D         mScissor;
  VkPipelineLayout mLayout;
  VkRenderPass     mRenderPass;
  VkPipeline       mGraphicsPipeline;
};

} // namespace FloraEngine
