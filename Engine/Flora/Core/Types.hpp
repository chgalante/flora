#include "pch.h"

namespace FloraEngine {

struct Vertex {
  glm::vec3 Position;
  glm::vec4 Colour;
  glm::vec2 TextureCoords;
  glm::vec1 TextureIdx;
};

} // namespace FloraEngine