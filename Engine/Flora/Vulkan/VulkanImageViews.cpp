#include "VulkanImageViews.hpp"

namespace FloraEngine {
VulkanImageViews::VulkanImageViews(VulkanDevice    *device,
                                   VulkanSwapChain *swap_chain)
    : pVulkanDeviceHandle(device), pVulkanSwapChainHandle(swap_chain) {}

VulkanImageViews::~VulkanImageViews() {}

void VulkanImageViews::Cleanup() {
  for (auto imageView : mSwapChainImageViews) {
    vkDestroyImageView(pVulkanDeviceHandle->GetDevice(), imageView, nullptr);
  }
}

void VulkanImageViews::Init() {
  create_vulkan_image_views();
}

void VulkanImageViews::create_vulkan_image_views() {
  const std::vector<VkImage> &swap_chain_images =
      pVulkanSwapChainHandle->GetSwapChainImages();
  const VkFormat &swap_chain_image_format =
      pVulkanSwapChainHandle->GetSwapChainImageFormat();

  mSwapChainImageViews.resize(swap_chain_images.size());

  for (size_t i = 0; i < swap_chain_images.size(); i++) {
    VkImageViewCreateInfo createInfo{};
    createInfo.sType        = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    createInfo.image        = swap_chain_images[i];
    createInfo.viewType     = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format       = swap_chain_image_format;
    createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
    createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;
    createInfo.subresourceRange.baseMipLevel   = 0;
    createInfo.subresourceRange.levelCount     = 1;
    createInfo.subresourceRange.baseArrayLayer = 0;
    createInfo.subresourceRange.layerCount     = 1;
    if (vkCreateImageView(pVulkanDeviceHandle->GetDevice(),
                          &createInfo,
                          nullptr,
                          &mSwapChainImageViews[i]) != VK_SUCCESS) {
      throw std::runtime_error("failed to create image views!");
    }
  }
}

std::vector<VkImageView> VulkanImageViews::GetImageViews() {
  return mSwapChainImageViews;
}

} // namespace FloraEngine
