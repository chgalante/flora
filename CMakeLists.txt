cmake_minimum_required(VERSION 3.20.5)
project (Garden CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Engine)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/Engine"
  "${CMAKE_CURRENT_SOURCE_DIR}/Engine/vendor/spdlog/include"
  "${CMAKE_CURRENT_SOURCE_DIR}/Garden"
)

add_executable(Garden
  "${CMAKE_CURRENT_SOURCE_DIR}/Garden/GardenApp.cpp"
)

target_link_libraries(Garden
  PUBLIC FloraEngine 
)
