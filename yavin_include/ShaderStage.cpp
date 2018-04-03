#include "ShaderStage.h"

#include "gl_includes.h"

namespace Yavin {

ShaderStage::ShaderStage(GLenum shaderType, const std::string &filename) {
  switch (shaderType) {
    case GL_VERTEX_SHADER: m_shader_type_name = "Vertex"; break;
    case GL_FRAGMENT_SHADER: m_shader_type_name = "Fragment"; break;
    case GL_GEOMETRY_SHADER: m_shader_type_name = "Geometry"; break;
    case GL_TESS_EVALUATION_SHADER:
      m_shader_type_name = "Tesselation Evaluation";
      break;
    case GL_TESS_CONTROL_SHADER:
      m_shader_type_name = "Tesselation Control";
      break;
    case GL_COMPUTE_SHADER: m_shader_type_name = "Compute"; break;
  }

  // Create Shader
  m_id = glCreateShader(shaderType);
  gl_error_check("glCreateShader");

  // parse file and extract variables
  auto source = ShaderStageParser::parse(filename, m_glsl_vars);

  const char *source_c = source.c_str();
  glShaderSource(id(), 1, &source_c, nullptr);
  gl_error_check("glShaderSource");

  // Compile Shader
  glCompileShader(id());
  gl_error_check("glCompileShader");
  print_log();
}

ShaderStage::ShaderStage(ShaderStage &&other)
    : m_id(other.m_id),
      m_shader_type_name(std::move(other.m_shader_type_name)),
      m_glsl_vars(std::move(other.m_glsl_vars)) {
  other.dont_delete = true;
}

ShaderStage::~ShaderStage() {
  if (!dont_delete) glDeleteShader(m_id);
}

const GLuint &ShaderStage::id() const { return m_id; }

void ShaderStage::print_log() {
  GLint   infologLength = 0;
  GLsizei charsWritten  = 0;

  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &infologLength);
  gl_error_check("glGetShaderiv");
  auto infoLog = new char[infologLength];
  glGetShaderInfoLog(id(), infologLength, &charsWritten, infoLog);
  gl_error_check("glGetShaderInfoLog");
  if (infologLength >= 1) {
    std::cout << "[Shader Compilation Error] - " << m_shader_type_name << '\n';
    std::cout << infoLog << '\n';
  }
  delete[] infoLog;
}
}  // namespace Yavin
