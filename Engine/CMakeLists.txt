cmake_minimum_required(VERSION 3.21.0)
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