#pragma once

#include <memory>

#if defined(FE_DEBUG)

#if (FE_PLATFORM_WINDOWS)
#define FE_DEBUGBREAK() __debugbreak()
#endif /* FE_PLATFORM_WINDOWS */

#if (FE_PLATFORM_UNIX)
#define FE_DEBUGBREAK() raise(SIGTRAP)
#endif /* FE_PLATFORM_WINDOWS */

#define FE_ENABLE_ASSERTS

#endif /* FE_DEBUG */

#define BIT(x) (1 << x)

#define FE_BIND_EVENT_FN(fn)                                \
  [this](auto &&...args) -> decltype(auto) {                \
    return this->fn(std::forward<decltype(args)>(args)...); \
  }

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