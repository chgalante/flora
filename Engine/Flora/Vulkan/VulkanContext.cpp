#include "Flora/Vulkan/VulkanContext.hpp"
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {

VulkanContext::VulkanContext(VulkanWindow *window) : pWindow(window) {}

VulkanContext::~VulkanContext() {}

void VulkanContext::Init() {

  /* Begin Vulkan renderer context initialization */
  FE_CORE_TRACE("Initializing Vulkan Instance...");

  /* Initialize instance */
  mVulkanInstance = CreateScope<VulkanInstance>(pWindow);
  mVulkanInstance->Init();

  /* Initialize device */
  mVulkanDevice = CreateScope<VulkanDevice>(mVulkanInstance.get());
  mVulkanDevice->Init();

  /* Initialize swap chain */
  mVulkanSwapChain = CreateScope<VulkanSwapChain>(mVulkanInstance.get(),
                                                  pWindow,
                                                  mVulkanDevice.get());
}

} // namespace FloraEngine