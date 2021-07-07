cmake_minimum_required(VERSION 3.20.5)
project (FloraEngine CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(vendor/glfw)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/Flora/
  ${CMAKE_CURRENT_SOURCE_DIR}/Application/
)

add_library(FloraEngine STATIC
  ${CMAKE_CURRENT_SOURCE_DIR}/Flora/Application/Application.cpp
  
)

link_libraries(FloraEngine
  PUBLIC glfw
)

# Platform Defines
if (WIN32)
  set(FE_PLATFORM_WINDOWS 1)
endif (WIN32)

if (UNIX AND NOT APPLE)
  set(FE_PLATFORM_UNIX 1)
endif (UNIX AND NOT APPLE)

if (APPLE)
  set(FE_PLATFORM_APPLE 1)
endif (APPLE)

# Configuration Defines
set(CMAKE_CXX_FLAGS_DEBUG "$(CMAKE_CXX_FLAGS_DEBUG) -DFE_DEBUG")