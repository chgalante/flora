#include "Flora/Vulkan/VulkanDevice.hpp"

namespace FloraEngine {

VulkanDevice::VulkanDevice(VulkanInstance *instance)
    : pVulkanInstanceHandle(instance) {}

VulkanDevice::~VulkanDevice() {
  vkDestroyDevice(mLogicalDevice, nullptr);
}

bool VulkanDevice::isCurrentDeviceSuitable() {

  UpdateQueueFamilies();
  UpdateSwapChainSupportDetails();
  /* TODO: prefer discrete over integrate GPU, for now any device with the
   * VK_QUEUE_GRAPHICS_BIT is chosen.
   */
  return mQueueFamilyIndices.isComplete() && checkDeviceExtensionSupport() &&
         checkSwapChainSupport();
}

void VulkanDevice::Init() {
  /* Update the physical device and device extensions */
  UpdatePhysicalDevice();

  std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
  std::set<uint32_t>                   uniqueQueueFamilies = {
      mQueueFamilyIndices.graphicsFamily.value(),
      mQueueFamilyIndices.presentFamily.value()};
  float queuePriority = 1.0f;
  for (uint32_t queueFamily : uniqueQueueFamilies) {
    VkDeviceQueueCreateInfo deviceQueueCreateInfo{};
    deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    deviceQueueCreateInfo.queueFamilyIndex = queueFamily;
    deviceQueueCreateInfo.queueCount       = 1;
    deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
    deviceQueueCreateInfos.push_back(deviceQueueCreateInfo);
  }

  VkDeviceCreateInfo deviceCreateInfo{};
  deviceCreateInfo.sType             = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  deviceCreateInfo.pQueueCreateInfos = deviceQueueCreateInfos.data();
  deviceCreateInfo.queueCreateInfoCount =
      static_cast<uint32_t>(deviceQueueCreateInfos.size());
  deviceCreateInfo.pEnabledFeatures = &mPhysicalDeviceFeatures;
  deviceCreateInfo.enabledExtensionCount =
      static_cast<uint32_t>(mDeviceExtensions.size());
  deviceCreateInfo.ppEnabledExtensionNames = mDeviceExtensions.data();

  if (vkCreateDevice(mPhysicalDevice,
                     &deviceCreateInfo,
                     nullptr,
                     &mLogicalDevice) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  /* Get device queue handles */
  vkGetDeviceQueue(mLogicalDevice,
                   mQueueFamilyIndices.graphicsFamily.value(),
                   0,
                   &mGraphicsQueue);
  vkGetDeviceQueue(mLogicalDevice,
                   mQueueFamilyIndices.presentFamily.value(),
                   0,
                   &mPresentQueue);
}

void VulkanDevice::UpdateQueueFamilies() {
  if (mPhysicalDevice == VK_NULL_HANDLE) {
    std::runtime_error(
        "VulkanDevice::mPhysicalDevice should have been initialized!");
  }
  uint32_t queueFamilyCount = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice,
                                           &queueFamilyCount,
                                           nullptr);

  std::vector<VkQueueFamilyProperties> queueFamilies =
      std::vector<VkQueueFamilyProperties>(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(mPhysicalDevice,
                                           &queueFamilyCount,
                                           queueFamilies.data());

  int                i = 0;
  QueueFamilyIndices indices;
  for (const auto &queueFamily : queueFamilies) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(mPhysicalDevice,
                                         i,
                                         pVulkanInstanceHandle->GetSurface(),
                                         &presentSupport);
    if (presentSupport) {
      indices.presentFamily = i;
    }
    if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
      indices.graphicsFamily = i;
    }
    if (indices.isComplete()) {
      break;
    }
    i++;
  }

  mQueueFamilyIndices = indices;
}

void VulkanDevice::UpdatePhysicalDevice() {

  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(pVulkanInstanceHandle->GetInstance(),
                             &deviceCount,
                             nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }
  /* At least one device with Vulkan support was found, now get the list of
   * physical devices */
  std::vector<VkPhysicalDevice> devices =
      std::vector<VkPhysicalDevice>(deviceCount);
  vkEnumeratePhysicalDevices(pVulkanInstanceHandle->GetInstance(),
                             &deviceCount,
                             devices.data());

  /* Check each device's suitability */
  for (const auto &device : devices) {
    mPhysicalDevice = device;
    vkGetPhysicalDeviceProperties(device, &mPhysicalDeviceProperties);
    vkGetPhysicalDeviceFeatures(device, &mPhysicalDeviceFeatures);
    if (isCurrentDeviceSuitable()) {
      return;
    }
  }

  /* If we reach here, a suitable device was not found */
  throw std::runtime_error("failed to find a suitable GPU!");
}

void VulkanDevice::UpdateSwapChainSupportDetails() {

  if (mPhysicalDevice == VK_NULL_HANDLE) {
    std::runtime_error(
        "VulkanDevice::mPhysicalDevice should have been initialized!");
  }

  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(
      mPhysicalDevice,
      pVulkanInstanceHandle->GetSurface(),
      &mSwapChainSupportDetails.capabilities);

  uint32_t formatCount;
  vkGetPhysicalDeviceSurfaceFormatsKHR(mPhysicalDevice,
                                       pVulkanInstanceHandle->GetSurface(),
                                       &formatCount,
                                       nullptr);
  if (formatCount != 0) {
    mSwapChainSupportDetails.formats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(
        mPhysicalDevice,
        pVulkanInstanceHandle->GetSurface(),
        &formatCount,
        mSwapChainSupportDetails.formats.data());
  }

  uint32_t presentModeCount;
  vkGetPhysicalDeviceSurfacePresentModesKHR(mPhysicalDevice,
                                            pVulkanInstanceHandle->GetSurface(),
                                            &presentModeCount,
                                            nullptr);
  if (presentModeCount != 0) {
    mSwapChainSupportDetails.presentModes.resize(presentModeCount);
    vkGetPhysicalDeviceSurfacePresentModesKHR(
        mPhysicalDevice,
        pVulkanInstanceHandle->GetSurface(),
        &presentModeCount,
        mSwapChainSupportDetails.presentModes.data());
  }
}

bool VulkanDevice::checkDeviceExtensionSupport() {

  FE_CORE_TRACE("Checking device extension support...");

  /* Get list of supported device extensions */
  uint32_t supportedDeviceExtensionCount = 0;
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice,
                                       nullptr,
                                       &supportedDeviceExtensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> supportedDeviceExtensions(
      supportedDeviceExtensionCount);
  vkEnumerateDeviceExtensionProperties(mPhysicalDevice,
                                       nullptr,
                                       &supportedDeviceExtensionCount,
                                       supportedDeviceExtensions.data());

  bool bExtensionsAreSupported = true;
  for (const auto &extension : mDeviceExtensions) {
    bool bExtensionSupportFound = false;
    for (const auto &supportedExtension : supportedDeviceExtensions) {
      if (!strcmp(extension, supportedExtension.extensionName)) {
        bExtensionSupportFound = true;
        break;
      }
    }
    if (bExtensionSupportFound) {
      FE_CORE_INFO("\t [*] {0}", extension);
    } else {
      FE_CORE_ERROR("\t [!] {0}", extension);
      bExtensionsAreSupported = false;
    }
  }
  return bExtensionsAreSupported;
}

bool VulkanDevice::checkSwapChainSupport() {
  return !mSwapChainSupportDetails.formats.empty() &&
         !mSwapChainSupportDetails.presentModes.empty();
}

SwapChainSupportDetails VulkanDevice::GetSwapChainSupportDetails() {
  return mSwapChainSupportDetails;
}

QueueFamilyIndices VulkanDevice::GetQueueFamilyIndices() {
  return mQueueFamilyIndices;
}

VkDevice VulkanDevice::GetDevice() {
  return mLogicalDevice;
}

} // namespace FloraEngine
