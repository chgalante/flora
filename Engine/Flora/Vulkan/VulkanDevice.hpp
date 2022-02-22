#pragma once
#include "pch.h"

#include "Flora/Vulkan/VulkanInstance.hpp"

#define VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME "VK_KHR_portability_subset"

namespace FloraEngine {

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};

struct SwapChainSupportDetails {
  VkSurfaceCapabilitiesKHR        capabilities;
  std::vector<VkSurfaceFormatKHR> formats;
  std::vector<VkPresentModeKHR>   presentModes;
};

class VulkanDevice {
public:
  VulkanDevice(VulkanInstance *);
  ~VulkanDevice();

  void Init();
  void Cleanup();

  SwapChainSupportDetails GetSwapChainSupportDetails();
  QueueFamilyIndices      GetQueueFamilyIndices();
  VkDevice                GetDevice();

private:
  void update_current_physical_device();
  void update_queue_families();
  void update_swap_chain_support_details();
  bool is_current_physical_device_suitable();
  bool check_device_extension_support();
  bool check_swap_chain_support();

private:
  VulkanInstance *pVulkanInstanceHandle;

  VkPhysicalDevice           mPhysicalDevice = VK_NULL_HANDLE;
  VkDevice                   mLogicalDevice  = VK_NULL_HANDLE;
  VkPhysicalDeviceFeatures   mPhysicalDeviceFeatures{};
  VkPhysicalDeviceProperties mPhysicalDeviceProperties{};
  QueueFamilyIndices         mQueueFamilyIndices;
  VkQueue                    mGraphicsQueue;
  VkQueue                    mPresentQueue;
  SwapChainSupportDetails    mSwapChainSupportDetails;

  const std::vector<const char *> mDeviceExtensions = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME};
};
} // namespace FloraEngine