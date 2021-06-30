#pragma once

#include "Application/Application.h"
#include "pch.h"

extern FloraEngine::Application *FloraEngine::CreateApplication();

int main() {
  auto app = FloraEngine::CreateApplication();
  app->Run();
  delete app;
}