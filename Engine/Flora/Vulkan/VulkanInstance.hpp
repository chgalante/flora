#pragma once

#include "Flora/Application/Window.hpp"

namespace FloraEngine {
class VulkanInstance {
public:
  VulkanInstance(Window *);
  ~VulkanInstance();

  void          Init();
  void          GetInstanceExtensions();
  VkInstance   *GetVkInstanceHandle();
  VkSurfaceKHR *GetVkSurfaceKHRHandle();

private:
  bool checkInstanceExtensionSupport();

private:
  Window                   *pWindow;
  VkInstance                mInstance;
  VkSurfaceKHR              mSurface;
  std::vector<const char *> mInstanceExtensions{
      "VK_KHR_get_physical_device_properties2"};

#ifdef FE_DEBUG
private:
  bool     checkValidationLayerSupport();
  VkResult CreateDebugUtilsMessengerEXT(
      const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
      const VkAllocationCallbacks              *pAllocator);

  void DestroyDebugUtilsMessengerEXT(const VkAllocationCallbacks *pAllocator);

private:
  VkDebugUtilsMessengerEXT        mDebugMessenger;
  const std::vector<const char *> mValidationLayers = {
      "VK_LAYER_KHRONOS_validation"};
#endif
};
} // namespace FloraEngine
