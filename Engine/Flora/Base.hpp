#pragma once

#include <memory>

/* FloraEngine Info */
#define FE_NAME          "FloraEngine"
#define FE_NAME_SHORT    "flora"
#define FE_VERSION_MAJOR 0
#define FE_VERSION_MINOR 1

#define GLFW_INCLUDE_VULKAN

#if defined(FE_DEBUG)
#if (FE_PLATFORM_WINDOWS)
#define FE_DEBUGBREAK() __debugbreak()
#endif /* FE_PLATFORM_WINDOWS */
#if (FE_PLATFORM_UNIX)
#define FE_DEBUGBREAK() raise(SIGTRAP)
#endif /* FE_PLATFORM_WINDOWS */
#define FE_ENABLE_ASSERTS
#endif /* FE_DEBUG */

namespace FloraEngine {
// FloraEngine::Ref
template <typename T> using Ref = std::shared_ptr<T>;
template <typename T, typename... Args>
constexpr Ref<T> CreateRef(Args &&...args) {
  return std::make_shared<T>(std::forward<Args>(args)...);
}
// FloraEngine::Scope
template <typename T> using Scope = std::unique_ptr<T>;
template <typename T, typename... Args>
constexpr Scope<T> CreateScope(Args &&...args) {
  return std::make_unique<T>(std::forward<Args>(args)...);
}

}; // namespace FloraEngine