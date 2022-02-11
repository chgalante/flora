#pragma once
#include "Flora/Vulkan/VulkanDevice.hpp"
#include "Flora/Vulkan/VulkanInstance.hpp"
#include "Flora/Vulkan/VulkanWindow.hpp"

namespace FloraEngine {
class VulkanSwapChain {
public:
  VulkanSwapChain(VulkanInstance *, VulkanWindow *, VulkanDevice *);
  ~VulkanSwapChain();

  void Init();
  void UpdateSurfaceFormat();
  void UpdatePresentMode();
  void UpdateExtent2D();
  void UpdateSwapChainImageHandles();

  VkSwapchainKHR       GetSwapChain();
  std::vector<VkImage> GetSwapChainImages();
  VkFormat             GetSwapChainFormat();
  VkExtent2D           GetSwapChainExtent2D();

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
