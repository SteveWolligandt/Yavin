#include "ShaderStage.h"

#include "gl_includes.h"

namespace Yavin {

ShaderStage::ShaderStage(GLenum             shader_type,
                         const std::string &filename_or_source,
                         StringType         string_type)
    : m_id(glCreateShader(shader_type)),
      m_shader_type(shader_type),
      m_string_type(string_type),
      m_filename_or_source(filename_or_source) {
  gl_error_check("glCreateShader");

  auto source =
      ShaderStageParser::parse(filename_or_source, m_glsl_vars, m_string_type);
  const char *source_c = source.c_str();
  glShaderSource(m_id, 1, &source_c, nullptr);
  gl_error_check("glShaderSource");

  // Compile Shader
  glCompileShader(id());
  gl_error_check("glCompileShader");
  print_log();
}

ShaderStage::ShaderStage(ShaderStage &&other)
    : m_id(other.m_id), m_glsl_vars(std::move(other.m_glsl_vars)) {
  other.dont_delete = true;
}

ShaderStage::~ShaderStage() {
  if (!dont_delete) glDeleteShader(m_id);
}

const GLuint &ShaderStage::id() const { return m_id; }

std::string ShaderStage::type_to_string(GLenum shader_type) {
  switch (shader_type) {
    case GL_VERTEX_SHADER: return "Vertex";
    case GL_FRAGMENT_SHADER: return "Fragment";
    case GL_GEOMETRY_SHADER: return "Geometry";
    case GL_TESS_EVALUATION_SHADER: return "Tesselation Evaluation";
    case GL_TESS_CONTROL_SHADER: return "Tesselation Control";
    case GL_COMPUTE_SHADER: return "Compute";
    default: return "unknown";
  }
}

std::string ShaderStage::stage() { return type_to_string(m_shader_type); }

void ShaderStage::print_log() {
  GLint   infologLength = 0;
  GLsizei charsWritten  = 0;

  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &infologLength);
  gl_error_check("glGetShaderiv");
  auto infoLog = new char[infologLength];
  glGetShaderInfoLog(id(), infologLength, &charsWritten, infoLog);
  gl_error_check("glGetShaderInfoLog");
  if (infologLength >= 1) {
    std::cout << "[Shader Compilation Error] - " << stage() << '\n';
    std::cout << infoLog << '\n';
  }
  delete[] infoLog;
}
}  // namespace Yavin
