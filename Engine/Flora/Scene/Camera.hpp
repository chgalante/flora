#pragma once

#include <glm/glm.hpp>

namespace FloraEngine {

class Camera {
public:
  Camera();
  ~Camera();

  void SetCameraSpeed(double speed);

  glm::mat4 GetCameraView();

private:
  double    mCameraSpeed;
  glm::vec3 mCameraPosition;
  glm::vec3 mCameraFront;
  glm::vec3 mCameraUp;
  glm::mat4 mCameraView;
  glm::mat4 mCameraProjection;
};

} // namespace FloraEngine