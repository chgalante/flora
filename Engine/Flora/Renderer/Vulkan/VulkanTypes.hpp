#pragma once
/* INCLUDES */
#include "pch.h"

/* PREPROCESSOR DEFINES */

/* Renderer Context */
#define CTX_INSTANCE              VkInstance mInstance;
#define CTX_WINDOW                Ref<Window> mWindow;
#define CTX_QUEUE_FAMILY_INDICES  QueueFamilyIndices mQueueFamilyIndices;
#define CTX_PHYSICAL_DEVICE       VkPhysicalDevice mPhysicalDevice = VK_NULL_HANDLE;
#define CTX_LOGICAL_DEVICE        VkDevice mLogicalDevice;
#define CTX_DEVICE_GRAPHICS_QUEUE VkQueue mGraphicsQueue;
#define CTX_DEVICE_PRESENT_QUEUE  VkQueue mPresentQueue;
#define CTX_SURFACE               VkSurfaceKHR mSurface;
#define CTX_DEBUG_MESSENGER_EXT   VkDebugUtilsMessengerEXT mDebugMessenger;
#define CTX_VALIDATION_LAYERS                           \
  const std::vector<const char *> mValidationLayers = { \
      "VK_LAYER_KHRONOS_validation"};

/* TYPE DEFINITIONS */

struct QueueFamilyIndices {
  std::optional<uint32_t> graphicsFamily;
  std::optional<uint32_t> presentFamily;

  bool isComplete() {
    return graphicsFamily.has_value() && presentFamily.has_value();
  }
};