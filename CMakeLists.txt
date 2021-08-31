cmake_minimum_required(VERSION 3.16.3)
project (Garden CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(Engine)

include_directories(
  "${CMAKE_CURRENT_SOURCE_DIR}/Engine"
  "${CMAKE_CURRENT_SOURCE_DIR}/Garden"
)

add_executable(Garden
  "${CMAKE_CURRENT_SOURCE_DIR}/Garden/Garden.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Garden/DebugLayer.cpp"
)

target_link_libraries(Garden
  PUBLIC FloraEngine 
)
