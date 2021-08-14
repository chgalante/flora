#include "Shader.hpp"
#include "Flora/Utilities/Log.hpp"

// GLAD
#include <glad/glad.h>

namespace FloraEngine {

static std::string ReadShaderSource(const std::string &path);

Shader::Shader(const std::string &vs_path, const std::string &fs_path) {
  unsigned int vertexShader, fragmentShader;
  std::string  vertexShaderString, fragmentShaderString;
  int          success;
  char         infoLog[512];

  vertexShaderString               = ReadShaderSource(vs_path);
  fragmentShaderString             = ReadShaderSource(fs_path);
  const char *vertexShaderSource   = vertexShaderString.c_str();
  const char *fragmentShaderSource = fragmentShaderString.c_str();

  vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
    FE_CORE_ERROR("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
    FE_CORE_TRACE(infoLog);
  }

  fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    FE_CORE_ERROR("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
    FE_CORE_TRACE(infoLog);
  }

  mRenderId = glCreateProgram();
  glAttachShader(mRenderId, vertexShader);
  glAttachShader(mRenderId, fragmentShader);
  glLinkProgram(mRenderId);
  glGetProgramiv(mRenderId, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(mRenderId, 512, NULL, infoLog);
    FE_CORE_ERROR("ERROR::SHADER::PROGRAM::LINKING_FAILED");
    FE_CORE_TRACE(infoLog);
  }

  /* Clean Up */
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);
  glDetachShader(mRenderId, vertexShader);
  glDetachShader(mRenderId, fragmentShader);
}

void Shader::Use() {
  glUseProgram(mRenderId);
}

static std::string ReadShaderSource(const std::string &path) {
  std::ostringstream sstream;
  std::ifstream      fs(path);
  sstream << fs.rdbuf();
  std::string str(sstream.str());
  return str;
}

} // namespace FloraEngine