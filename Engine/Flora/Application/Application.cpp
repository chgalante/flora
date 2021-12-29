#include "Application.hpp"

namespace FloraEngine {

Application::Application() {
  /* Initialize Members */
  mLayers           = CreateScope<std::vector<ApplicationLayer *>>();
  mIsRunning        = true;
  mLayerInsertIndex = 0;
}

Application::~Application() {
  vkDestroyInstance(mVkInstance, nullptr);
}

void Application::Run() {
  uint32_t     glfwExtensionCount = 0;
  const char **glfwExtensions =
      glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
  uint32_t                           extensionCount = 0;
  std::vector<VkExtensionProperties> extensions;

  /* Init Application Window */
  mWindow = CreateScope<Window>();

  /* Init Vulkan */
  FE_CORE_TRACE("Initializing Vulkan Instance...");
  VkApplicationInfo appInfo{};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "HelloTriangle";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "FloraEngine";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_0;

  VkInstanceCreateInfo createInfo{};
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = &appInfo;
  createInfo.enabledExtensionCount   = glfwExtensionCount;
  createInfo.ppEnabledExtensionNames = glfwExtensions;
  createInfo.enabledLayerCount       = 0;

  if (vkCreateInstance(&createInfo, nullptr, &mVkInstance) != VK_SUCCESS) {
    throw std::runtime_error("failed to create vulkan instance!");
  }

  /* Get Instance Extension Info */
  vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
  extensions = std::vector<VkExtensionProperties>(extensionCount);
  vkEnumerateInstanceExtensionProperties(nullptr,
                                         &extensionCount,
                                         extensions.data());
  FE_CORE_TRACE("Available extensions: ");
  for (const auto &extension : extensions) {
    FE_CORE_TRACE("\t{0}", extension.extensionName);
  }

  /* Attach each of the application layers */
  for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
    (*mLayers)[idx]->OnAttach();
  }

  /* App Core */
  while (IsRunning()) {
    /* Exit from application core loop when the window should close */
    if (!mWindow->OnUpdate()) {
      mIsRunning = false;
      break;
    }

    /* Update each of the application layers */
    for (int32_t idx = mLayers->size() - 1; idx >= 0; idx--) {
      (*mLayers)[idx]->OnUpdate();
    }
  }
}

/* ApplicationLayer Stack:
 * ----------------------------------------------
 * Overlay3     <- insert overlay   vector back
 * Overlay2                               |
 * Overlay1                               | dir
 * Layer3       <- insert layer           |
 * Layer2                                 V
 * Layer1                           vector front
 * ----------------------------------------------
 *
 * Overlays and layers are processed for updates
 * from the back to the front of the underlying
 * vector container. Overlays are placed on top
 * of the stack, layers are placed after the
 * last overlay.
 */

void Application::PushLayer(ApplicationLayer *application_layer) {

  /* Push layer on layer stack */
  mLayers->emplace(mLayers->begin() + mLayerInsertIndex, application_layer);
  mLayerInsertIndex++;
}

void Application::PushOverlay(ApplicationLayer *application_overlay) {

  /* Push overlay on top of overlay stack */
  mLayers->emplace_back(application_overlay);
}

bool Application::IsRunning() {
  return mIsRunning;
}

} // namespace FloraEngine