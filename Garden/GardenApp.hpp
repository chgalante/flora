#include "FloraEngine.hpp"

class Garden : public FloraEngine::Application {
public:
  Garden() {}
  ~Garden() {}

  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();
};