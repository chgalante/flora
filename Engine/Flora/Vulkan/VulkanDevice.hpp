#include "pch.h"

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
  VulkanDevice(VkInstance *, VkSurfaceKHR *);
  ~VulkanDevice();

  void Init();
  void UpdatePhysicalDevice();
  void UpdateQueueFamilies();
  void UpdateSwapChainSupportDetails();

private:
  bool isCurrentDeviceSuitable();
  bool checkDeviceExtensionSupport();
  bool checkSwapChainSupport();

private:
  VkInstance   *pInstanceHandle;
  VkSurfaceKHR *pSurfaceHandle;

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