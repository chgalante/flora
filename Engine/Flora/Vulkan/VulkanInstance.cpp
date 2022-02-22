#include "Flora/Vulkan/VulkanInstance.hpp"
#include "pch.h"

namespace FloraEngine {

VulkanInstance::VulkanInstance(VulkanWindow *window) : pWindow(window) {}

VulkanInstance::~VulkanInstance() {}

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT             messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void                                       *pUserData) {

  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

VkInstance VulkanInstance::GetInstance() {
  return mInstance;
}

VkSurfaceKHR VulkanInstance::GetSurface() {
  return mSurface;
}

void VulkanInstance::Cleanup() {
#ifdef FE_DEBUG
  destroy_debug_utils_messenger(nullptr);
#endif
  vkDestroyInstance(mInstance, nullptr);
}

void VulkanInstance::Init() {
  /* Create app info structure */
  VkApplicationInfo appInfo{};
  appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = FE_NAME;
  appInfo.applicationVersion =
      VK_MAKE_VERSION(FE_VERSION_MAJOR, 0, FE_VERSION_MINOR);
  appInfo.pEngineName = FE_NAME;
  appInfo.engineVersion =
      VK_MAKE_VERSION(FE_VERSION_MAJOR, 0, FE_VERSION_MINOR);
  appInfo.apiVersion = VK_API_VERSION_1_0;

  /* Create instance create info and select instance extensions */
  VkInstanceCreateInfo instanceCreateInfo{};
  update_instance_extensions();
  instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  instanceCreateInfo.enabledExtensionCount   = mInstanceExtensions.size();
  instanceCreateInfo.ppEnabledExtensionNames = &mInstanceExtensions[0];
  instanceCreateInfo.enabledLayerCount       = 0;
  instanceCreateInfo.pNext                   = nullptr;

#ifdef FE_DEBUG
  /* Create debug messenger and add validation layers to the
   * instance if compiled for debug */
  VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
  debugUtilsMessengerCreateInfo.sType =
      VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  debugUtilsMessengerCreateInfo.messageSeverity =
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  debugUtilsMessengerCreateInfo.messageType =
      VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
      VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  debugUtilsMessengerCreateInfo.pfnUserCallback = debugCallback;
  debugUtilsMessengerCreateInfo.pNext           = nullptr;

  /* Only register validation layers in the instance create info if the selected
   * validation layers are supported */
  if (!check_validation_layer_support()) {
    throw std::runtime_error("validation layers requested, but not available!");
  }
  instanceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(mValidationLayers.size());
  instanceCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
  instanceCreateInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
#endif

  /* Check that the instance extensions selected for flora would supported by
   * the instance */
  if (!check_instance_extension_support()) {
    throw std::runtime_error("one or more required extensions are not "
                             "supported by the vulkan instance!");
  }

  /* Create the vulkan instance */
  if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vulkan instance!");
  }

#ifdef FE_DEBUG
  /* Create the debug messenger */
  if (create_debug_utils_messenger(&debugUtilsMessengerCreateInfo, nullptr) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
#endif

  /* Create a surface */
  if (glfwCreateWindowSurface(mInstance,
                              pWindow->GetGLFWHandle(),
                              nullptr,
                              &mSurface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

void VulkanInstance::update_instance_extensions() {

  /* Get list of required extensions for glfw */
  uint32_t     glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  for (int i = 0; i < glfwExtensionCount; i++) {
    mInstanceExtensions.push_back(glfwExtensions[i]);
  }
#ifdef FE_DEBUG
  /* If the build target is debug, include debug utils extension as part of
   * required extensions
   */
  mInstanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif
}

bool VulkanInstance::check_instance_extension_support() {

  /* Check that required extensions are supported */
  FE_CORE_TRACE("Checking instance extension support... ");

  uint32_t supportedExtensionCount = 0;
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedExtensionCount,
                                         nullptr);
  std::vector<VkExtensionProperties> supportedExtensions(
      supportedExtensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &supportedExtensionCount,
                                         supportedExtensions.data());

  bool bExtensionsAreSupported = true;
  for (const auto &requiredExtension : mInstanceExtensions) {
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
bool VulkanInstance::check_validation_layer_support() {
  uint32_t                       layerCount;
  std::vector<VkLayerProperties> availableLayers;

  /* Check that Vulkan Validation Layers are supported */
  FE_CORE_TRACE("Validating vulkan validation layers... ");

  vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
  availableLayers = std::vector<VkLayerProperties>(layerCount);
  vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

  for (const char *layerName : mValidationLayers) {
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

VkResult VulkanInstance::create_debug_utils_messenger(
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator) {
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance,
      "vkCreateDebugUtilsMessengerEXT");
  if (func != nullptr) {
    return func(mInstance, pCreateInfo, pAllocator, &mDebugMessenger);
  } else {
    return VK_ERROR_EXTENSION_NOT_PRESENT;
  }
}

void VulkanInstance::destroy_debug_utils_messenger(
    const VkAllocationCallbacks *pAllocator) {
  auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(
      mInstance,
      "vkDestroyDebugUtilsMessengerEXT");
  if (func != nullptr) {
    func(mInstance, mDebugMessenger, pAllocator);
  }
}
#endif

} // namespace FloraEngine
