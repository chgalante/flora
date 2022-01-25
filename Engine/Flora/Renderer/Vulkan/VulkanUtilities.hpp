#pragma once
/* INCLUDES */
#include "VulkanTypes.hpp"
#include "pch.h"

/* PUBLIC FUNCTION PROTOTYPES */
std::vector<const char *> GetInstanceExtensions();
std::vector<const char *> GetDeviceExtensions(VkPhysicalDevice &);
VkPhysicalDevice          GetPhysicalDevice(VkInstance &);
QueueFamilyIndices        GetQueueFamilies(VkPhysicalDevice device);

bool CheckValidationLayerSupport(
    const std::vector<const char *> validationLayers);
bool CheckInstanceExtensionSupport(
    std::vector<const char *> requiredExtensions);

#ifdef FE_DEBUG
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks              *pAllocator,
    VkDebugUtilsMessengerEXT                 *pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     debugMessenger,
                                   const VkAllocationCallbacks *pAllocator);
#endif