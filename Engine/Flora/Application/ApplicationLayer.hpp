#pragma once
#include "Event/Event.hpp"
#include "pch.h"

namespace FloraEngine {

class ApplicationLayer {

public:
  ApplicationLayer();
  ~ApplicationLayer();

  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();
  virtual void OnEvent(Event &e);

  void SetEventCallback(std::function<bool()> callback);
};

} // namespace FloraEngine