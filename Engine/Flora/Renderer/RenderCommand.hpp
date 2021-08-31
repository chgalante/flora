#pragma once
#include "Scene/Mesh.hpp"
#include "Shader.hpp"

namespace FloraEngine {

class RenderCommand {

public:
  RenderCommand() {}
  ~RenderCommand() {}

  static void DrawIndices(Mesh *mesh, Shader *shader);
};
} // namespace FloraEngine