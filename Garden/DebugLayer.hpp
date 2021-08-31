#pragma once
#include "FloraEngine.hpp"

class DebugLayer : public FloraEngine::ApplicationLayer {
public:
  DebugLayer() : ApplicationLayer() {}
  ~DebugLayer() {}

  void OnEvent(FloraEngine::Event &e) override;

  void OnAttach() override;
  void OnUpdate() override;
  void OnDetach() override;

  bool OnKeyPressed(FloraEngine::KeyPressedEvent &e);
};