#include "Flora/Vulkan/VulkanSwapChain.hpp"

namespace FloraEngine {

VulkanSwapChain::VulkanSwapChain(VulkanInstance *instance,
                                 VulkanWindow   *window,
                                 VulkanDevice   *device)
    : pVulkanInstanceHandle(instance), pVulkanWindowHandle(window),
      pVulkanDeviceHandle(device) {}

VulkanSwapChain::~VulkanSwapChain() {}

void VulkanSwapChain::Cleanup() {
  vkDestroySwapchainKHR(pVulkanDeviceHandle->GetDevice(), mSwapChain, nullptr);
}

void VulkanSwapChain::Init() {

  SwapChainSupportDetails swapChainSupport =
      pVulkanDeviceHandle->GetSwapChainSupportDetails();
  QueueFamilyIndices queueFamilyIndices =
      pVulkanDeviceHandle->GetQueueFamilyIndices();

  update_surface_format();
  update_present_mode();
  update_extent_2d();

  uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
  if (swapChainSupport.capabilities.maxImageCount > 0 &&
      imageCount > swapChainSupport.capabilities.maxImageCount) {
    imageCount = swapChainSupport.capabilities.maxImageCount;
  }

  VkSwapchainCreateInfoKHR createInfo{};
  createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  createInfo.surface          = pVulkanInstanceHandle->GetSurface();
  createInfo.minImageCount    = imageCount;
  createInfo.imageFormat      = mSurfaceFormat.format;
  createInfo.imageColorSpace  = mSurfaceFormat.colorSpace;
  createInfo.imageExtent      = mExtent2D;
  createInfo.imageArrayLayers = 1;
  createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

  if (queueFamilyIndices.graphicsFamily != queueFamilyIndices.presentFamily) {
    createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
    createInfo.queueFamilyIndexCount = 2;
    createInfo.pQueueFamilyIndices   = (uint32_t *)(&queueFamilyIndices);
  } else {
    createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.queueFamilyIndexCount = 0;
    createInfo.pQueueFamilyIndices   = nullptr;
  }

  createInfo.preTransform   = swapChainSupport.capabilities.currentTransform;
  createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
  createInfo.presentMode    = mPresentMode;
  createInfo.clipped        = VK_TRUE;
  createInfo.oldSwapchain   = VK_NULL_HANDLE;

  if (vkCreateSwapchainKHR(pVulkanDeviceHandle->GetDevice(),
                           &createInfo,
                           nullptr,
                           &mSwapChain) != VK_SUCCESS) {
    throw std::runtime_error("failed to create swap chain!");
  }

  update_swap_chain_image_handles();
}

void VulkanSwapChain::update_surface_format() {
  SwapChainSupportDetails swapChainSupport =
      pVulkanDeviceHandle->GetSwapChainSupportDetails();
  for (const auto &availableFormat : swapChainSupport.formats) {
    if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
        availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
      mSurfaceFormat = availableFormat;
    }
  }
  /* If preferred format is not found, settle for the first supported format
   */
  mSurfaceFormat = swapChainSupport.formats[0];
}

void VulkanSwapChain::update_present_mode() {
  SwapChainSupportDetails swapChainSupport =
      pVulkanDeviceHandle->GetSwapChainSupportDetails();
  for (const auto &availablePresentMode : swapChainSupport.presentModes) {
    if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
      mPresentMode = availablePresentMode;
    }
  }
  /* If preferred present mode is not found, settle for
   * VK_PRESENT_MODE_FIFO_KHR which is guaranteed to be available.*/
  mPresentMode = VK_PRESENT_MODE_FIFO_KHR;
}

void VulkanSwapChain::update_extent_2d() {
  SwapChainSupportDetails swapChainSupport =
      pVulkanDeviceHandle->GetSwapChainSupportDetails();
  if (swapChainSupport.capabilities.currentExtent.width != UINT32_MAX) {
    mExtent2D = swapChainSupport.capabilities.currentExtent;
  } else {
    mExtent2D = {pVulkanWindowHandle->GetWidth(),
                 pVulkanWindowHandle->GetHeight()};
    mExtent2D.width =
        std::max(swapChainSupport.capabilities.minImageExtent.width,
                 std::min(swapChainSupport.capabilities.maxImageExtent.width,
                          mExtent2D.width));
    mExtent2D.height =
        std::max(swapChainSupport.capabilities.minImageExtent.height,
                 std::min(swapChainSupport.capabilities.maxImageExtent.height,
                          mExtent2D.height));
  }
}

void VulkanSwapChain::update_swap_chain_image_handles() {
  uint32_t imageCount;
  vkGetSwapchainImagesKHR(pVulkanDeviceHandle->GetDevice(),
                          mSwapChain,
                          &imageCount,
                          nullptr);
  mSwapChainImages.resize(imageCount);
  vkGetSwapchainImagesKHR(pVulkanDeviceHandle->GetDevice(),
                          mSwapChain,
                          &imageCount,
                          mSwapChainImages.data());
}

VkSwapchainKHR VulkanSwapChain::GetSwapChain() {
  return mSwapChain;
}

std::vector<VkImage> VulkanSwapChain::GetSwapChainImages() {
  return mSwapChainImages;
}

VkFormat VulkanSwapChain::GetSwapChainImageFormat() {
  return mSurfaceFormat.format;
}

VkExtent2D VulkanSwapChain::GetSwapChainExtent2D() {
  return mExtent2D;
}

} // namespace FloraEngine
