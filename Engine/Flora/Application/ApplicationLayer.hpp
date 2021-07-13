#pragma once

namespace FloraEngine {

class ApplicationLayer {
public:
  ApplicationLayer();
  ~ApplicationLayer(){};

  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();
};
} // namespace FloraEngine