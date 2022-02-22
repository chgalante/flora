#pragma once

#include "Flora/Vulkan/VulkanWindow.hpp"

namespace FloraEngine {
class VulkanInstance {
public:
  VulkanInstance(VulkanWindow *);
  ~VulkanInstance();

  void         Init();
  void         Cleanup();
  VkInstance   GetInstance();
  VkSurfaceKHR GetSurface();

private:
  void update_instance_extensions();
  bool check_instance_extension_support();
#ifdef FE_DEBUG
  bool     check_validation_layer_support();
  VkResult create_debug_utils_messenger(
      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
      const VkAllocationCallbacks              *pAllocator);
  void destroy_debug_utils_messenger(const VkAllocationCallbacks *pAllocator);
#endif

private:
  VulkanWindow *pWindow;

  VkInstance   mInstance;
  VkSurfaceKHR mSurface;

  std::vector<const char *> mInstanceExtensions{
      "VK_KHR_get_physical_device_properties2"};

#ifdef FE_DEBUG
  VkDebugUtilsMessengerEXT        mDebugMessenger;
  const std::vector<const char *> mValidationLayers = {
      "VK_LAYER_KHRONOS_validation"};
#endif
};
} // namespace FloraEngine
