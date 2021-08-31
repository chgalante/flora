#pragma once
#include "FloraEngine.hpp"

class DebugLayer : public FloraEngine::ApplicationLayer {
public:
  DebugLayer() : ApplicationLayer() {}
  ~DebugLayer() {}

  virtual void OnEvent(FloraEngine::Event &e) override;
  virtual void OnAttach() override;
  virtual void OnUpdate() override;
  virtual void OnDetach() override;
};