/* INCLUDES */
#include "Flora/Base.hpp"
#include "Flora/Renderer/RendererContext.hpp"
#include "VulkanTypes.hpp"
#include "VulkanUtilities.hpp"
#include "pch.h"

/* STATIC FUNCTIONS */
#ifdef FE_DEBUG
static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT             messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void                                       *pUserData) {

  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}
#endif

static void populateDebugMessengerCreateInfo(
    VkDebugUtilsMessengerCreateInfoEXT  &createInfo,
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
    VkInstanceCreateInfo            &createInfo,
    VkApplicationInfo               *appInfo,
    const std::vector<const char *> &instanceExtensions) {
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = appInfo;
  createInfo.enabledExtensionCount   = instanceExtensions.size();
  createInfo.ppEnabledExtensionNames = &instanceExtensions[0];
  createInfo.enabledLayerCount       = 0;
  createInfo.pNext                   = nullptr;
}

static void populateQueueCreateInfo(VkDeviceQueueCreateInfo  &createInfo,
                                    const QueueFamilyIndices &indices,
                                    float                     queuePriority) {
  createInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  createInfo.queueFamilyIndex = indices.graphicsFamily.value();
  createInfo.queueCount       = 1;
  createInfo.pQueuePriorities = &queuePriority;
}

static void populateDeviceCreateInfo(
    VkDeviceCreateInfo              &createInfo,
    VkDeviceQueueCreateInfo         *queueCreateInfo,
    VkPhysicalDeviceFeatures        *deviceFeatures,
    const std::vector<const char *> &deviceExtensions) {
  createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  createInfo.pQueueCreateInfos       = queueCreateInfo;
  createInfo.queueCreateInfoCount    = 1;
  createInfo.pEnabledFeatures        = deviceFeatures;
  createInfo.enabledExtensionCount   = deviceExtensions.size();
  createInfo.ppEnabledExtensionNames = &deviceExtensions[0];
}

/* METHOD IMPLEMENTATIONS */

namespace FloraEngine {

RendererContext::RendererContext() {}

void RendererContext::Init() {
  std::vector<const char *> instanceExtensions;
  std::vector<const char *> deviceExtensions;
  VkApplicationInfo         appInfo{};
  VkInstanceCreateInfo      instanceCreateInfo{};
  VkDeviceQueueCreateInfo   queueCreateInfo{};
  VkPhysicalDeviceFeatures  deviceFeatures{};
  VkDeviceCreateInfo        deviceCreateInfo{};

  /* Init Vulkan */
  FE_CORE_TRACE("Initializing Vulkan Instance...");

  instanceExtensions = GetInstanceExtensions();

  populateApplicationInfo(appInfo, FE_NAME, FE_NAME);
  populateInstanceCreateInfo(instanceCreateInfo, &appInfo, instanceExtensions);

#ifdef FE_DEBUG
  VkDebugUtilsMessengerCreateInfoEXT debugUtilsMessengerCreateInfo{};
  populateDebugMessengerCreateInfo(debugUtilsMessengerCreateInfo,
                                   debugCallback);

  /* If defined validation layers are supported, register those validation
   * layers in instance create info
   */
  if (!CheckValidationLayerSupport(mValidationLayers)) {
    throw std::runtime_error("validation layers requested, but not available!");
  }
  instanceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(mValidationLayers.size());
  instanceCreateInfo.ppEnabledLayerNames = mValidationLayers.data();
  instanceCreateInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
#endif

  /* Check that the extensions required by flora are supported by the instance
   */
  if (!CheckInstanceExtensionSupport(instanceExtensions)) {
    throw std::runtime_error("one or more required extensions are not "
                             "supported by the vulkan instance!");
  }

  /* Create vulkan instance from instance create info structure */
  if (vkCreateInstance(&instanceCreateInfo, nullptr, &mInstance) !=
      VK_SUCCESS) {
    throw std::runtime_error("failed to create vulkan instance!");
  }

#ifdef FE_DEBUG

  if (CreateDebugUtilsMessengerEXT(mInstance,
                                   &debugUtilsMessengerCreateInfo,
                                   nullptr,
                                   &mDebugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
#endif

  /* Select a physical device */
  mPhysicalDevice = GetPhysicalDevice(mInstance);

  /* Get Device Extensions */
  deviceExtensions = GetDeviceExtensions(mPhysicalDevice);

  /* Create logical device(s) */
  mQueueFamilyIndices = GetQueueFamilies(mPhysicalDevice);
  populateQueueCreateInfo(queueCreateInfo, mQueueFamilyIndices, 1.0f);
  populateDeviceCreateInfo(deviceCreateInfo,
                           &queueCreateInfo,
                           &deviceFeatures,
                           deviceExtensions);

  if (vkCreateDevice(mPhysicalDevice,
                     &deviceCreateInfo,
                     nullptr,
                     &mLogicalDevice) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }

  vkGetDeviceQueue(mLogicalDevice,
                   mQueueFamilyIndices.graphicsFamily.value(),
                   0,
                   &mGraphicsQueue);
}

void RendererContext::Cleanup() {
#ifdef FE_DEBUG
  DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
  vkDestroyDevice(mLogicalDevice, nullptr);
  vkDestroyInstance(mInstance, nullptr);
}

RendererContext::~RendererContext() {
  Cleanup();
}

} // namespace FloraEngine