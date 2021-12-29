#pragma once

#include "Flora/Application/Application.hpp"
#include "Flora/Base.hpp"
#include "Flora/Utilities/Log.hpp"
#include "pch.h"

#define GLFW_INCLUDE_VULKAN

extern FloraEngine::Application *FloraEngine::CreateApplication();

int main(int argc, char **argv) {

  FloraEngine::Log::Init();

  /* Create Application */
  FloraEngine::Application *pApp = FloraEngine::CreateApplication();

  try {
    /* Start FloraEngine */
    FE_CORE_INFO("Starting FloraEngine...");

    /* Start Application Core */
    pApp->Run();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  /* Exit Application */
  return EXIT_SUCCESS;
}