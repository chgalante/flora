#pragma once

#include "pch.h"

namespace FloraEngine {

class Shader {
public:
  Shader(const std::string &vertex_source, const std::string &fragment_source);
  ~Shader();

  void Use();

private:
  unsigned int mRenderId;
};

} // namespace FloraEngine