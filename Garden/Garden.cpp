#include "Garden.hpp"
#include "DebugLayer.hpp"

Garden::Garden() : FloraEngine::Application() {
  FE_INFO("Garden App Started!");
  PushLayer(new DebugLayer());
}

FloraEngine::Application *FloraEngine::CreateApplication() {
  return new Garden();
}