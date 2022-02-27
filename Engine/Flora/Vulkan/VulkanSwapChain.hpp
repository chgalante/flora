#pragma once
#include "Flora/Vulkan/VulkanDevice.hpp"
#include "Flora/Vulkan/VulkanInstance.hpp"
#include "Flora/Vulkan/VulkanWindow.hpp"

namespace FloraEngine {
class VulkanSwapChain {
public:
  VulkanSwapChain(VulkanWindow *, VulkanInstance *, VulkanDevice *);
  ~VulkanSwapChain();

  void Init();
  void Cleanup();

  VkSwapchainKHR       GetSwapChain();
  std::vector<VkImage> GetSwapChainImages();
  VkFormat             GetSwapChainImageFormat();
  VkExtent2D           GetSwapChainExtent2D();

private:
  void update_surface_format();
  void update_present_mode();
  void update_extent_2d();
  void update_swap_chain_image_handles();

private:
  VulkanInstance *pVulkanInstanceHandle;
  VulkanWindow   *pVulkanWindowHandle;
  VulkanDevice   *pVulkanDeviceHandle;

  VkSurfaceFormatKHR mSurfaceFormat;
  VkPresentModeKHR   mPresentMode;
  VkExtent2D         mExtent2D;
  VkSwapchainKHR     mSwapChain;

  std::vector<VkImage> mSwapChainImages;
};
} // namespace FloraEngine
