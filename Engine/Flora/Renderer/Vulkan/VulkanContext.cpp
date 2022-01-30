/* INCLUDES */
#include "Flora/Base.hpp"
#include "Flora/Renderer/RendererContext.hpp"
#include "VulkanTypes.hpp"
#include "VulkanUtilities.hpp"
#include "pch.h"

namespace FloraEngine {

/* STATIC FUNCTIONS */

/* Callback Functions */

static VKAPI_ATTR VkBool32 VKAPI_CALL
debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      messageSeverity,
              VkDebugUtilsMessageTypeFlagsEXT             messageType,
              const VkDebugUtilsMessengerCallbackDataEXT *pCallbackData,
              void                                       *pUserData) {

  std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;

  return VK_FALSE;
}

/* Initialization Functions */

static void initializeInstance(
    VkInstance                      &instance,
    const std::vector<const char *> &validationLayers,
    VkDebugUtilsMessengerEXT        &debugMessenger) {

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
  VkInstanceCreateInfo      instanceCreateInfo{};
  std::vector<const char *> instanceExtensions = GetInstanceExtensions();
  instanceCreateInfo.sType            = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  instanceCreateInfo.pApplicationInfo = &appInfo;
  instanceCreateInfo.enabledExtensionCount   = instanceExtensions.size();
  instanceCreateInfo.ppEnabledExtensionNames = &instanceExtensions[0];
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
  if (!CheckValidationLayerSupport(validationLayers)) {
    throw std::runtime_error("validation layers requested, but not available!");
  }
  instanceCreateInfo.enabledLayerCount =
      static_cast<uint32_t>(validationLayers.size());
  instanceCreateInfo.ppEnabledLayerNames = validationLayers.data();
  instanceCreateInfo.pNext =
      (VkDebugUtilsMessengerCreateInfoEXT *)&debugUtilsMessengerCreateInfo;
#endif

  /* Check that the instance extensions selected for flora would supported by
   * the instance */
  if (!CheckInstanceExtensionSupport(instanceExtensions)) {
    throw std::runtime_error("one or more required extensions are not "
                             "supported by the vulkan instance!");
  }

  /* Create the vulkan instance */
  if (vkCreateInstance(&instanceCreateInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create vulkan instance!");
  }

#ifdef FE_DEBUG
  /* Create the debug messenger */
  if (CreateDebugUtilsMessengerEXT(instance,
                                   &debugUtilsMessengerCreateInfo,
                                   nullptr,
                                   &debugMessenger) != VK_SUCCESS) {
    throw std::runtime_error("failed to set up debug messenger!");
  }
#endif
}

static void initializeSurface(VkInstance   &instance,
                              Ref<Window>  &window,
                              VkSurfaceKHR &surface) {
  /* Create a surface */
  if (glfwCreateWindowSurface(instance,
                              window->GetHandle(),
                              nullptr,
                              &surface) != VK_SUCCESS) {
    throw std::runtime_error("failed to create window surface!");
  }
}

static void initializeDevice(VkInstance               &instance,
                             VkSurfaceKHR             &surface,
                             VkPhysicalDevice         &physicalDevice,
                             QueueFamilyIndices       &queueFamilyIndices,
                             VkPhysicalDeviceFeatures &physicalDeviceFeatures,
                             VkDevice                 &logicalDevice) {
  /* Select a physical device and device extensions */
  physicalDevice = GetPhysicalDevice(instance, surface);
  std::vector<const char *> deviceExtensions =
      GetDeviceExtensions(physicalDevice);

  /* Create logical device and device queues */
  queueFamilyIndices = GetQueueFamilies(physicalDevice, surface);

  std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
  std::set<uint32_t>                   uniqueQueueFamilies = {
      queueFamilyIndices.graphicsFamily.value(),
      queueFamilyIndices.presentFamily.value()};
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
  deviceCreateInfo.pEnabledFeatures        = &physicalDeviceFeatures;
  deviceCreateInfo.enabledExtensionCount   = deviceExtensions.size();
  deviceCreateInfo.ppEnabledExtensionNames = &deviceExtensions[0];

  if (vkCreateDevice(physicalDevice,
                     &deviceCreateInfo,
                     nullptr,
                     &logicalDevice) != VK_SUCCESS) {
    throw std::runtime_error("failed to create logical device!");
  }
}

/* METHOD IMPLEMENTATIONS */

RendererContext::RendererContext(Ref<Window> window) : mWindow(window) {}

void RendererContext::Init() {

  /* Begin Vulkan renderer context initialization */
  FE_CORE_TRACE("Initializing Vulkan Instance...");

  /* Initialize instance */
  initializeInstance(mInstance, mValidationLayers, mDebugMessenger);

  /* Initialize surface */
  initializeSurface(mInstance, mWindow, mSurface);

  /* Initialize device and device queues */
  initializeDevice(mInstance,
                   mSurface,
                   mPhysicalDevice,
                   mQueueFamilyIndices,
                   mPhysicalDeviceFeatures,
                   mLogicalDevice);

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

void RendererContext::Cleanup() {
#ifdef FE_DEBUG
  DestroyDebugUtilsMessengerEXT(mInstance, mDebugMessenger, nullptr);
#endif
  vkDestroyDevice(mLogicalDevice, nullptr);
  vkDestroySurfaceKHR(mInstance, mSurface, nullptr);
  vkDestroyInstance(mInstance, nullptr);
}

RendererContext::~RendererContext() {
  Cleanup();
}

} // namespace FloraEngine