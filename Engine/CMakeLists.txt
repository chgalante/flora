cmake_minimum_required(VERSION 3.20.5)
project (FloraEngine C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Don't build GLFW tests, examples and documentation
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

# Platform Defines
if (WIN32)
  set(FE_PLATFORM_WINDOWS 1)
endif (WIN32)

if (UNIX AND NOT APPLE)
  set(FE_PLATFORM_UNIX 1)
endif (UNIX AND NOT APPLE)

if (APPLE)
  set(FE_PLATFORM_APPLE 1)
  set(GFX_CONTEXT_SOURCES 
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Renderer/Vulkan/VulkanContext.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Renderer/Vulkan/VulkanUtilities.cpp"
  )
endif (APPLE)

# Add subdirectories
add_subdirectory(vendor/glfw)

# FloraEngine Static Library
add_library(FloraEngine STATIC
  "${CMAKE_CURRENT_SOURCE_DIR}/FloraEngine.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Application/Application.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Application/ApplicationLayer.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Application/Window.cpp"

  "${GFX_CONTEXT_SOURCES}"

  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Render/RenderCommand.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Render/Shader.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Render/Camera.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/Flora/Render/Mesh.cpp"

  "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/src/glad.c"
)

target_precompile_headers(FloraEngine PRIVATE
  "$<$<COMPILE_LANGUAGE:CXX>:${CMAKE_CURRENT_SOURCE_DIR}/pch.h>"
  "$<$<COMPILE_LANGUAGE:C>:<stddef.h$<ANGLE-R>>"
  "$<$<COMPILE_LANGUAGE:CXX>:<cstddef$<ANGLE-R>>"
)

# Includes
target_include_directories(FloraEngine
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw/include/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/spdlog/include/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glad/include/"
  PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm/"
)

# Find packages
# find_package(OpenGL REQUIRED)
find_package(Vulkan REQUIRED FATAL_ERROR)

# link libraries
target_link_libraries(FloraEngine
  glfw
  Vulkan::Vulkan
  Vulkan::Headers
)



# Configuration Defines
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DFE_DEBUG")