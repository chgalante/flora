#include "DebugLayer.hpp"
#include "Scene/Camera.hpp"

FloraEngine::Camera camera;

void DebugLayer::OnAttach() {
  FE_TRACE("DebugLayer Attached!");
  camera = FloraEngine::Camera();
}

void DebugLayer::OnUpdate() {}

void DebugLayer::OnDetach() {
  FE_TRACE("DebugLayer Detached!");
}

void DebugLayer::OnEvent(FloraEngine::Event &e) {
  FloraEngine::EventDispatcher dispatcher = FloraEngine::EventDispatcher(e);

  /* DEBUG PRINT */
  FE_TRACE(e.ToString());

  /* KeyPressedEvent */
  dispatcher.Dispatch<FloraEngine::KeyPressedEvent>(
      FE_BIND_EVENT_FN(DebugLayer::OnKeyPressed));
}

bool DebugLayer::OnKeyPressed(FloraEngine::KeyPressedEvent &e) {
  return true;
}