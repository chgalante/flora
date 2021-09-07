#pragma once
#include "Mesh.hpp"

namespace FloraEngine {

class Scene {
public:
  Scene() {}
  ~Scene() {}

private:
  MeshList mMeshList;
}
} // namespace FloraEngine