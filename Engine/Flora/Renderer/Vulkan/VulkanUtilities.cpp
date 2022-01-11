#include "VulkanUtilities.hpp"

/* Function Implementations */

std::vector<const char *> GetRequiredExtensions() {
  /* Get list of required extensions for glfw */
  uint32_t     glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  std::vector<const char *> extensions(glfwExtensions,
                                       glfwExtensions + glfwExtensionCount);

#ifdef FE_DEBUG
  /* If the build target is debug, include debug utils extension as part of
   * required extensions
   */
  extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
  return extensions;
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

bool CheckExtensionSupport(std::vector<const char *> requiredExtensions) {
  uint32_t                           supportedExtensionCount = 0;
  std::vector<VkExtensionProperties> supportedExtensions;
  bool                               bExtensionsAreSupported = true;

  /* Check that required extensions are supported */
  FE_CORE_TRACE("Validating required extensions... ");

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
      if (strcmp(requiredExtension, supportedExtension.extensionName)) {
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
    const VkAllocationCallbacks *             pAllocator,
    VkDebugUtilsMessengerEXT *                pDebugMessenger) {
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