#include "DebugLayer.hpp"

static FloraEngine::Mesh *  mesh;
static FloraEngine::Shader *shader;

void DebugLayer::OnAttach() {
  FE_TRACE("DebugLayer Attached!");
  mesh   = new FloraEngine::Mesh();
  shader = new FloraEngine::Shader("../Engine/Flora/Resources/vertex.vs",
                                   "../Engine/Flora/Resources/fragment.fs");
}

void DebugLayer::OnUpdate() {
  /* Do Something, not nothing */

  FloraEngine::RenderCommand::DrawIndices(mesh, shader);
}

void DebugLayer::OnDetach() {
  FE_TRACE("DebugLayer Detached!");
}

void DebugLayer::OnEvent(FloraEngine::Event &e) {
  FloraEngine::EventDispatcher e_dispatcher = FloraEngine::EventDispatcher(e);

  /* WindowClosedEvent */
  e_dispatcher.Dispatch(FloraEngine::EventType::WindowClosedEvent, [&e]() {
    FE_TRACE(e.ToString());
    return true;
  });

  /* WindowResizedEvent */
  e_dispatcher.Dispatch(FloraEngine::EventType::WindowResizedEvent, [&e]() {
    FE_TRACE(e.ToString());
    return true;
  });

  /* WindowMovedEvent */
  e_dispatcher.Dispatch(FloraEngine::EventType::WindowMovedEvent, [&e]() {
    FE_TRACE(e.ToString());
    return true;
  });

  /* WindowLostFocusEvent */
  e_dispatcher.Dispatch(FloraEngine::EventType::WindowLostFocusEvent, [&e]() {
    FE_TRACE(e.ToString());
    return true;
  });
}