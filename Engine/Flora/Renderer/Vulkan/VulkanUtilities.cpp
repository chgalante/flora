/* INCLUDES */
#include "VulkanUtilities.hpp"

/* STATIC FUNCTIONS */
static bool isDeviceSuitable(VkPhysicalDevice device, VkSurfaceKHR surface) {
  VkPhysicalDeviceProperties deviceProperties;
  VkPhysicalDeviceFeatures   deviceFeatures;
  QueueFamilyIndices         queueFamilyIndices;

  vkGetPhysicalDeviceProperties(device, &deviceProperties);
  vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

  queueFamilyIndices = GetQueueFamilies(device, surface);

  /* TODO: prefer discrete over integrate GPU, for now any device with the
   * VK_QUEUE_GRAPHICS_BIT is chosen.
   */
  return queueFamilyIndices.isComplete();
}

/* PUBLIC FUNCTION IMPLEMENTATIONS */

std::vector<const char *> GetInstanceExtensions() {

  /* Get list of required extensions for glfw */
  uint32_t     glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

  /* Get list of supported instance extensions */
  uint32_t supportedInstanceExtensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedInstanceExtensionCount,
                                         nullptr);
  std::vector<VkExtensionProperties> supportedInstanceExtensions(
      supportedInstanceExtensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedInstanceExtensionCount,
                                         supportedInstanceExtensions.data());

  const char *optionalExtension0 = "VK_KHR_get_physical_device_properties2";
  for (const auto &instanceExtension : supportedInstanceExtensions) {
    /* Register VK_KHR_get_physical_device_properties2 as a required extension
     * if it is supported */
    if (!strcmp(instanceExtension.extensionName, optionalExtension0)) {
      extensions.push_back(optionalExtension0);
    }
  }

#ifdef FE_DEBUG
  /* If the build target is debug, include debug utils extension as part of
   * required extensions
   */
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  return extensions;
}

std::vector<const char *> GetDeviceExtensions(VkPhysicalDevice &device) {

  std::vector<const char *> extensions;

  /* Get list of supported device extensions */
  uint32_t supportedDeviceExtensionCount = 0;
  vkEnumerateDeviceExtensionProperties(device,
                                       nullptr,
                                       &supportedDeviceExtensionCount,
                                       nullptr);
  std::vector<VkExtensionProperties> supportedDeviceExtensions(
      supportedDeviceExtensionCount);
  vkEnumerateDeviceExtensionProperties(device,
                                       nullptr,
                                       &supportedDeviceExtensionCount,
                                       supportedDeviceExtensions.data());

  const char *optionalExtension0 = "VK_KHR_portability_subset";
  for (const auto &deviceExtension : supportedDeviceExtensions) {
    /* Register VK_KHR_portability_subset as a required extension
     * if it is supported */
    if (!strcmp(deviceExtension.extensionName, optionalExtension0)) {
      extensions.push_back(optionalExtension0);
    }
  }
  return extensions;
}

VkPhysicalDevice GetPhysicalDevice(VkInstance &instance, VkSurfaceKHR surface) {
  uint32_t                      deviceCount = 0;
  std::vector<VkPhysicalDevice> devices;

  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

  if (deviceCount == 0) {
    throw std::runtime_error("failed to find GPUs with Vulkan support!");
  }
  /* At least one device with Vulkan support was found, now get the list of
   * physical devices */
  devices = std::vector<VkPhysicalDevice>(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  /* Check each device's suitability */
  for (const auto &device : devices) {
    if (isDeviceSuitable(device, surface)) {
      return device;
    }
  }

  /* If we reach here, a suitable device was not found */
  throw std::runtime_error("failed to find a suitable GPU!");
}

QueueFamilyIndices GetQueueFamilies(VkPhysicalDevice device,
                                    VkSurfaceKHR     surface) {
  QueueFamilyIndices                   indices;
  uint32_t                             queueFamilyCount = 0;
  std::vector<VkQueueFamilyProperties> queueFamilies;

  vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

  queueFamilies = std::vector<VkQueueFamilyProperties>(queueFamilyCount);
  vkGetPhysicalDeviceQueueFamilyProperties(device,
                                           &queueFamilyCount,
                                           queueFamilies.data());

  int i = 0;
  for (const auto &queueFamily : queueFamilies) {
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);
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

  return indices;
}

bool CheckValidationLayerSupport(
    const std::vector<const char *> validationLayers) {
  uint32_t                       layerCount;
  std::vector<VkLayerProperties> availableLayers;

  /* Check that Vulkan Validation Layers are supported */
  FE_CORE_TRACE("Validating vulkan validation layers... ");

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  availableLayers = std::vector<VkLayerProperties>(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : validationLayers) {
    bool layerFound = false;

    for (const auto &layerProperties : availableLayers) {
      if (strcmp(layerName, layerProperties.layerName) == 0) {
        layerFound = true;
        break;
      }
    }

    if (!layerFound) {
      return false;
    }
  }
  return true;
}

bool CheckInstanceExtensionSupport(
    std::vector<const char *> requiredExtensions) {
  uint32_t                           supportedExtensionCount = 0;
  std::vector<VkExtensionProperties> supportedExtensions;
  bool                               bExtensionsAreSupported = true;

  /* Check that required extensions are supported */
  FE_CORE_TRACE("Checking instance extension support... ");

  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedExtensionCount,
                                         nullptr);
  supportedExtensions =
      std::vector<VkExtensionProperties>(supportedExtensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedExtensionCount,
                                         supportedExtensions.data());

  for (const auto &requiredExtension : requiredExtensions) {
    bool bExtensionSupportFound = false;
    for (const auto &supportedExtension : supportedExtensions) {
      if (!strcmp(requiredExtension, supportedExtension.extensionName)) {
        bExtensionSupportFound = true;
        break;
      }
    }
    if (bExtensionSupportFound) {
      FE_CORE_INFO("\t [*] {0}", requiredExtension);
    } else {
      FE_CORE_ERROR("\t [!] {0}", requiredExtension);
      bExtensionsAreSupported = false;
    }
  }
  return bExtensionsAreSupported;
}

#ifdef FE_DEBUG
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator,
    VkDebugUtilsMessengerEXT                 *pDebugMessenger) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     debugMessenger,
                                   const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      instance,
      "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(instance, debugMessenger, pAllocator);
  }
}
#endif