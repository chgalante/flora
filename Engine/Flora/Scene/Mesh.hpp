#pragma once
#include "Core/Types.hpp"
#include "pch.h"

namespace FloraEngine {
class Mesh {
public:
  Mesh() {}
  ~Mesh() {}

  void LoadOBJ(const std::string &path);

private:
  std::vector<Vertex> mVertices;
};
} // namespace FloraEngine
