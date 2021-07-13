#include "GardenApp.hpp"

void Garden::OnAttach() {
  FE_TRACE("Garden Attached!");
}

void Garden::OnUpdate() {
  FE_TRACE("Garden Updated!");
}

void Garden::OnDetach() {
  FE_TRACE("Garden Detached!");
}

FloraEngine::Application *FloraEngine::CreateApplication() {
  return new Garden();
}