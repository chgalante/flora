#pragma once
#include "VulkanDevice.hpp"

namespace FloraEngine {
class VulkanGraphicsPipeline {
public:
  VulkanGraphicsPipeline(VulkanDevice *);
  ~VulkanGraphicsPipeline();

  void Init();
  void Cleanup();

  void UpdateShaderModules();

private:
  void CompileGLSLtoSPIRV();

private:
  VulkanDevice  *pVulkanDeviceHandle;
  VkShaderModule mVertexShaderModule;
  VkShaderModule mFragmentShaderModule;
};

} // namespace FloraEngine
