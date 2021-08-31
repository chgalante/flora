#pragma once

#include <glm/glm.hpp>

class Camera {
public:
  Camera();
  ~Camera();

private:
  glm::mat4 mViewMatrix;
  glm::mat4 mProjectionMatrix;
};