#include "FloraEngine.h"

class Garden : public FloraEngine::Application {
public:
  Garden() {}
  ~Garden() {}
};

FloraEngine::Application *FloraEngine::CreateApplication() {
  return new Garden();
}