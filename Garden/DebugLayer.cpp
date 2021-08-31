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