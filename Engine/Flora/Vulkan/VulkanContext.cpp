#include "Flora/Vulkan/VulkanContext.hpp"
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {

VulkanContext::VulkanContext() {}

VulkanContext::~VulkanContext() {
  mVulkanFramebuffers->Cleanup();
  mVulkanGraphicsPipeline->Cleanup();
  mVulkanImageViews->Cleanup();
  mVulkanSwapChain->Cleanup();
  mVulkanDevice->Cleanup();
  mVulkanInstance->Cleanup();
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
  mVulkanSwapChain = CreateScope<VulkanSwapChain>(mVulkanWindow.get(),
                                                  mVulkanInstance.get(),
                                                  mVulkanDevice.get());
  mVulkanSwapChain->Init();

  /* Initialize image views */
  mVulkanImageViews = CreateScope<VulkanImageViews>(mVulkanDevice.get(),
                                                    mVulkanSwapChain.get());
  mVulkanImageViews->Init();

  /* Initialize graphics pipeline */
  mVulkanGraphicsPipeline =
      CreateScope<VulkanGraphicsPipeline>(mVulkanDevice.get(),
                                          mVulkanSwapChain.get());
  mVulkanGraphicsPipeline->Init();

  /* Initialize framebuffers */
  mVulkanFramebuffers =
      CreateScope<VulkanFramebuffers>(mVulkanDevice.get(),
                                      mVulkanSwapChain.get(),
                                      mVulkanImageViews.get(),
                                      mVulkanGraphicsPipeline.get());
  mVulkanFramebuffers->Init();
}

} // namespace FloraEngine