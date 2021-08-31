#pragma once
#include "Events/Event.hpp"
#include "pch.h"

namespace FloraEngine {

class ApplicationLayer {

public:
  ApplicationLayer();
  ~ApplicationLayer();

  virtual void OnEvent(Event &e);

  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();

  void SetEventCallback(std::function<bool()> callback);
};

} // namespace FloraEngine