#include "ApplicationLayer.hpp"

namespace FloraEngine {

ApplicationLayer::ApplicationLayer() {}

ApplicationLayer::~ApplicationLayer() {
  this->OnDetach();
}

void ApplicationLayer::OnAttach() {}
void ApplicationLayer::OnUpdate() {}
void ApplicationLayer::OnDetach() {}

} // namespace FloraEngine