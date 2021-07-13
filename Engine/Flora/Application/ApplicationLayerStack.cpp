#include "ApplicationLayerStack.hpp"

namespace FloraEngine {

ApplicationLayerStack::ApplicationLayerStack() {
  iterTop = vecApplicationLayerStack.begin();
}

void ApplicationLayerStack::PushLayer(Ref<ApplicationLayer> application_layer) {
  vecApplicationLayerStack.insert(iterTop, application_layer);
}
void ApplicationLayerStack::PushOverlay(
    Ref<ApplicationLayer> application_layer) {
  vecApplicationLayerStack.insert(iterTop + 1, application_layer);
}
}