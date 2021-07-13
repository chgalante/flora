#pragma once

#include "ApplicationLayer.hpp"
#include "Flora/Base.hpp"
#include "pch.h"

namespace FloraEngine {
class ApplicationLayerStack {
public:
  ApplicationLayerStack();
  ~ApplicationLayerStack() {}

public:
  void PushLayer(Ref<ApplicationLayer> application_layer);
  void PushOverlay(Ref<ApplicationLayer> application_layer);

private:
  std::vector<Ref<ApplicationLayer>>::iterator iterTop;
  std::vector<Ref<ApplicationLayer>>           vecApplicationLayers;
};
} // namespace FloraEngine
