#pragma once

#include "Flora/Application/Application.h"
#include "Flora/Base.h"
#include "Flora/Utilities/Log.h"
#include "pch.h"

extern FloraEngine::Application *FloraEngine::CreateApplication();

int main(int argc, char **argv) {

  FloraEngine::Log::Init();

  /* Start FloraEngine */
  FE_CORE_INFO("FloraEngine Started!");

  /* Create and Run Application */
  auto app = FloraEngine::CreateApplication();
  app->Run();
  delete app;
}