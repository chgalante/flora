#include "Camera.hpp"

namespace FloraEngine {
Camera::Camera() {
  mCameraPosition = glm::vec3(0.0f, 0.0f, 0.0f);
  mCameraFront    = glm::vec3(0.0f, 0.0f, -1.0f);
  mCameraUp       = glm::vec3(0.0f, 1.0f, 0.0f);

  mCameraProjection =
      glm::perspective(45.0f, (float)1280 / (float)720, 1.0f, 150.0f);
}

Camera::~Camera() {}

void Camera::SetCameraSpeed(double speed) {
  mCameraSpeed = speed;
}

glm::mat4 Camera::GetCameraView() {
  mCameraView =
      glm::lookAt(mCameraPosition, mCameraPosition + mCameraFront, mCameraUp);
  return mCameraView;
}

} // namespace FloraEngine