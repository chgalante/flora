#include "Flora/Base.hpp"
#include "Flora/Renderer/GraphicsContext.hpp"
#include "VulkanUtilities.hpp"
#include "pch.h"

namespace FloraEngine {

/* Static Variables */
#ifdef FE_DEBUG
static VkDebugUtilsMessengerEXT        sDebugMessenger;
static const std::vector<const char *> sValidationLayers = {
    "VK_LAYER_KHRONOS_validation"};
#endif

/* Static Functions */
#ifdef FE_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT             messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void *                                      pUserData) {

  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}
#endif

static void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT & createInfo,
    PFN_vkDebugUtilsMessengerCallbackEXT callback) {
  createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                               VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                           VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  createInfo.pfnUserCallback = callback;
  createInfo.pNext           = nullptr;
};

static void populateApplicationInfo(VkApplicationInfo &appInfo,
                                    const std::string &appName,
                                    const std::string &engineName) {
  appInfo.sType            = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName = appName.c_str();
  appInfo.applicationVersion =
      VK_MAKE_VERSION(FE_VERSION_MAJOR, 0, FE_VERSION_MINOR);
  appInfo.pEngineName = engineName.c_str();
  appInfo.engineVersion =
      VK_MAKE_VERSION(FE_VERSION_MAJOR, 0, FE_VERSION_MINOR);
  appInfo.apiVersion = VK_API_VERSION_1_0;
}

static void populateInstanceCreateInfo(
    VkInstanceCreateInfo &           createInfo,
    VkApplicationInfo *              appInfo,
    const std::vector<const char *> &requiredExtensions) {
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = appInfo;
  createInfo.enabledExtensionCount   = requiredExtensions.size();
  createInfo.ppEnabledExtensionNames = &requiredExtensions[0];
  createInfo.enabledLayerCount       = 0;
  createInfo.pNext                   = nullptr;
}

GraphicsContext::GraphicsContext() {}

void GraphicsContext::Init() {
  std::vector<const char *> requiredExtensions = GetRequiredExtensions();
  VkApplicationInfo         appInfo{};
  VkInstanceCreateInfo      instanceCreateInfo{};

  /* Init Vulkan */
  FE_CORE_TRACE("Initializing Vulkan Instance...");

  populateApplicationInfo(appInfo, FE_NAME, FE_NAME);
  populateInstanceCreateInfo(instanceCreateInfo, &appInfo, requiredExtensions);

#ifdef FE_DEBUG
  VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
  populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo,
                                   debugCallback);

  /* If defined validation layers are supported, register those validation
   * layers in instance create info
   */
  if (!CheckValidationLayerSupport(sValidationLayers)) {
    throw std::runtime_error("validation layers requested, but not available!");
  }
  instanceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(sValidationLayers.size());
  instanceCreateInfo.ppEnabledLayerNames = sValidationLayers.data();
  instanceCreateInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
#endif

  /* Create vulkan instance from instance create info structure */
  if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vulkan instance!");
  }

#ifdef FE_DEBUG

  if (CreateDebugUtilsMessengerEXT(mInstance,
                                   &debugUtilsMessengerCreateInfo,
                                   nullptr,
                                   &sDebugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
#endif

  /* Check that the extensions required by flora are supported by the instance
   */
  if (!CheckExtensionSupport(requiredExtensions)) {
    throw std::runtime_error("one or more required extensions are not "
                             "supported by the vulkan instance!");
  }
}

void GraphicsContext::Cleanup() {
#ifdef FE_DEBUG
  DestroyDebugUtilsMessengerEXT(mInstance, sDebugMessenger, nullptr);
#endif
  vkDestroyInstance(mInstance, nullptr);
}

GraphicsContext::~GraphicsContext() {
  Cleanup();
}

} // namespace FloraEngine