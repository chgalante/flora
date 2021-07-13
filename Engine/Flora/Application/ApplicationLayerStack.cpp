#include "ApplicationLayerStack.hpp"

namespace FloraEngine {

ApplicationLayerStack::ApplicationLayerStack() {
  iterTop = vecApplicationLayers.begin();
}

void ApplicationLayerStack::PushLayer(Ref<ApplicationLayer> application_layer) {
  vecApplicationLayers.insert(iterTop, application_layer);
}
void ApplicationLayerStack::PushOverlay(
    Ref<ApplicationLayer> application_layer) {
  vecApplicationLayers.insert(iterTop + 1, application_layer);
}
}