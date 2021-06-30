#pragma once

#include "pch.h"

namespace FloraEngine {

class Application {
public:
  Application() {
    b_IsRunning = true;
  }

  ~Application() {}

  bool IsRunning() {
    return b_IsRunning;
  }

  void Run() {
    while (IsRunning()) {
      std::cout << "FloraEngine!" << std::endl;
    }
  }

  bool b_IsRunning = false;
};

/* To be defined by client */
Application *CreateApplication();

} // namespace FloraEngine