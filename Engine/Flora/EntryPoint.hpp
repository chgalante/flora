#pragma once
#include "Flora/Application/Application.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"
#include "pch.h"

extern FloraEngine::Application *FloraEngine::CreateApplication();

__attribute__((weak)) int main(int argc, char **argv) {

  FloraEngine::Log::Init();

  /* Start FloraEngine */
  FE_CORE_INFO("FloraEngine Started!");

  /* Create Application */
  FloraEngine::Application *pApp = FloraEngine::CreateApplication();

  /* Start Application Core */
  pApp->Run();

  return 0;
}