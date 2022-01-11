#include "pch.h"

/* Function Prototypes */

std::vector<const char *> GetRequiredExtensions();

bool CheckValidationLayerSupport(
    const std::vector<const char *> validationLayers);
bool CheckExtensionSupport(std::vector<const char *> requiredExtensions);

#ifdef FE_DEBUG
VkResult CreateDebugUtilsMessengerEXT(
    VkInstance                                instance,
    const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *             pAllocator,
    VkDebugUtilsMessengerEXT *                pDebugMessenger);

void DestroyDebugUtilsMessengerEXT(VkInstance                   instance,
                                   VkDebugUtilsMessengerEXT     debugMessenger,
                                   const VkAllocationCallbacks *pAllocator);
#endif