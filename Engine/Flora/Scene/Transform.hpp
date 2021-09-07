#pragma once
#include "pch.h"

namespace FloraEngine {

class Transform {
public:
  Transform() {}
  ~Transform() {}

  glm::mat4 Get();

private:
  glm::mat4 mTransformMatrix;
};
} // namespace FloraEngine