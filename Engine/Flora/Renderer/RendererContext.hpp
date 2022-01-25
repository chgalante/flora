#pragma once
#include "pch.h"

#ifndef FE_OPENGL_API
#include "Vulkan/VulkanTypes.hpp"
#else
#include "OpenGL/OpenGLTypes.hpp"
#endif

namespace FloraEngine {

class RendererContext {
public:
  RendererContext();
  ~RendererContext();

  void Init();
  void Cleanup();

private:
  CTX_INSTANCE
  CTX_QUEUE_FAMILY_INDICES
  CTX_PHYSICAL_DEVICE
  CTX_LOGICAL_DEVICE
  CTX_DEVICE_GRAPHICS_QUEUE
  CTX_SURFACE
  CTX_DEBUG_MESSENGER_EXT
  CTX_VALIDATION_LAYERS
};
} // namespace FloraEngine