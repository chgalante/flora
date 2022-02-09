#include "Flora/Vulkan/VulkanContext.hpp"
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {

VulkanContext::VulkanContext(Window *window) : pWindow(window) {}

VulkanContext::~VulkanContext() {}

void VulkanContext::Init() {

  /* Begin Vulkan renderer context initialization */
  FE_CORE_TRACE("Initializing Vulkan Instance...");

  /* Initialize instance */
  mVulkanInstance = CreateScope<VulkanInstance>(pWindow);
  mVulkanInstance->Init();

  /* Initialize device */
  mVulkanDevice =
      CreateScope<VulkanDevice>(mVulkanInstance.get()->GetVkInstanceHandle(),
                                mVulkanInstance.get()->GetVkSurfaceKHRHandle());
  mVulkanDevice->Init();
}

} // namespace FloraEngine