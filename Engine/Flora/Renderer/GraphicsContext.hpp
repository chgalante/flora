#pragma once
#include "pch.h"

namespace FloraEngine {
#ifdef FE_OPENGL_API
#define GFX_API_INSTANCE_TYPE unsigned int
#else
#define GFX_API_INSTANCE_TYPE VkInstance
#endif

class GraphicsContext {
public:
  GraphicsContext();
  ~GraphicsContext();

  void Init();
  void Cleanup();

private:
  GFX_API_INSTANCE_TYPE mInstance;
};
} // namespace FloraEngine