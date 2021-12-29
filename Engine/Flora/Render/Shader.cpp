#include "Shader.hpp"
#include "Flora/Utilities/Log.hpp"

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
}

static std::string ReadShaderSource(const std::string &path) {
  std::ostringstream sstream;
  std::ifstream      fs(path);
  sstream << fs.rdbuf();
  std::string str(sstream.str());
  return str;
}

} // namespace FloraEngine