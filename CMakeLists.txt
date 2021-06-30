cmake_minimum_required(VERSION 3.21.0)
project (Garden CXX)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(Engine)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/Engine/
  ${CMAKE_CURRENT_SOURCE_DIR}/Garden/
)

add_executable(Garden
  ${CMAKE_CURRENT_SOURCE_DIR}/Garden/GardenApp.cpp
)

link_libraries(Garden
  PUBLIC FloraEngine 
)