#include "GardenApp.hpp"
#include "FloraEngine.hpp"

static FloraEngine::Mesh *  mesh;
static FloraEngine::Shader *shader;

class RenderLayer : public FloraEngine::ApplicationLayer {
public:
  RenderLayer() : ApplicationLayer() {}
  ~RenderLayer() {}

  virtual void OnAttach() override {
    FE_TRACE("RenderLayer Attached!");
    mesh   = new FloraEngine::Mesh();
    shader = new FloraEngine::Shader("../Engine/Flora/Resources/vertex.vs",
                                     "../Engine/Flora/Resources/fragment.fs");
  }

  virtual void OnUpdate() override {
    /* Do Something, not nothing */

    FloraEngine::RenderCommand::DrawIndices(mesh, shader);
  }

  virtual void OnDetach() override {
    FE_TRACE("RenderLayer Detached!");
  }
};

class UILayer : public FloraEngine::ApplicationLayer {
public:
  UILayer() : ApplicationLayer() {}
  ~UILayer() {}

  virtual void OnAttach() override {
    FE_TRACE("UILayer Attached!");
  }

  virtual void OnUpdate() override {
    /* Do Something, not nothing */
  }

  virtual void OnDetach() override {
    FE_TRACE("UILayer Detached!");
  }
};

Garden::Garden() : Application() {
  FE_TRACE("Garden App Started!");
  PushLayer(new RenderLayer());
  PushOverlay(new UILayer());
}

FloraEngine::Application *FloraEngine::CreateApplication() {
  return new Garden();
}