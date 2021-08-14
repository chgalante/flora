#pragma once

#include "pch.h"

namespace FloraEngine {
class Mesh {
public:
  Mesh() {}
  ~Mesh() {}

  float mVertices[9] =
      {-0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f, 0.5f, 0.0f};
  unsigned int VBO;
  unsigned int VAO;
};
} // namespace FloraEngine
