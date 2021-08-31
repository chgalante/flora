#pragma once
#include "FloraEngine.hpp"

class Garden : public FloraEngine::Application {
public:
  Garden();
  ~Garden() {}

  virtual inline void OnAttach() override {}
  virtual inline void OnDetach() override {}
  virtual inline void OnUpdate() override {}
};