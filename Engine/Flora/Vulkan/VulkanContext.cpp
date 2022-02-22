#include "Flora/Vulkan/VulkanContext.hpp"
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {

VulkanContext::VulkanContext() {}

VulkanContext::~VulkanContext() {
  mVulkanInstance->Cleanup();
  mVulkanDevice->Cleanup();
  mVulkanSwapChain->Cleanup();
}

VulkanWindow *VulkanContext::GetWindow() {
  return mVulkanWindow.get();
}

void VulkanContext::Init() {

  /* Begin Vulkan renderer context initialization */
  FE_CORE_TRACE("Initializing Vulkan Context...");

  /* Initialize window */
  mVulkanWindow = CreateScope<VulkanWindow>();

  /* Initialize instance */
  mVulkanInstance = CreateScope<VulkanInstance>(mVulkanWindow.get());
  mVulkanInstance->Init();

  /* Initialize device */
  mVulkanDevice = CreateScope<VulkanDevice>(mVulkanInstance.get());
  mVulkanDevice->Init();

  /* Initialize swap chain */
  mVulkanSwapChain = CreateScope<VulkanSwapChain>(mVulkanInstance.get(),
                                                  mVulkanWindow.get(),
                                                  mVulkanDevice.get());
  mVulkanSwapChain->Init();

  /* Initialize image views */
  mVulkanImageViews = CreateScope<VulkanImageViews>(mVulkanSwapChain.get(),
                                                    mVulkanDevice.get());
  mVulkanImageViews->Init();

  /* Initialize graphics pipeline */
  mVulkanGraphicsPipeline = CreateScope<VulkanGraphicsPipeline>();
  mVulkanGraphicsPipeline->Init();
}

} // namespace FloraEngine