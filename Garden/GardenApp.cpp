#include "FloraEngine.h"

class Garden : public FloraEngine::Application {
public:
  Garden() {}
  ~Garden() {}

  virtual void OnAttach();
  virtual void OnUpdate();
  virtual void OnDetach();

};

void Garden::OnAttach() {
  FE_TRACE("Garden Attached!");
}

void Garden::OnUpdate(){
  FE_TRACE("Garden Updated!");
}

void Garden::OnDetach(){
 FE_TRACE("Garden Detached!");
}

FloraEngine::Application *FloraEngine::CreateApplication() {
  return new Garden();
}