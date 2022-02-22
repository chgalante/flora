#pragma once

#include "Flora/Application/Application.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"
#include "pch.h"

extern FloraEngine::Application *FloraEngine::CreateApplication();

int main(int argc, char **argv) {

  try {
    /* Start FloraEngine */
    FloraEngine::Log::Init();
    FE_CORE_TRACE("Starting FloraEngine...");

    /* Create Application */
    FloraEngine::Application *pApp = FloraEngine::CreateApplication();

    /* Start Application Core */
    pApp->Run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  /* Exit Application */
  return EXIT_SUCCESS;
}