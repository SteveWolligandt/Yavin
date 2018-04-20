#include "Shader.h"

#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

template void Shader::add_shader_stage<VertexShader, char const*>(char const*);
template void Shader::add_shader_stage<FragmentShader, char const*>(
    char const*);
template void Shader::add_shader_stage<GeometryShader, char const*>(
    char const*);
template void
              Shader::add_shader_stage<TesselationEvaluationShader, char const*>(char const*);
template void Shader::add_shader_stage<TesselationControlShader, char const*>(
    char const*);
template void Shader::add_shader_stage<ComputeShader, char const*>(char const*);

//------------------------------------------------------------------------------

void Shader::create() {
  delete_shader();
  m_id = glCreateProgram();
  gl_error_check("glCreateProgram");

  for (auto& stage : m_shader_stages) {
    stage.compile();
    glAttachShader(m_id, stage.id());
    gl_error_check("glAttachShader - " + stage.stage_name());
    for (const auto& var : stage.glsl_vars()) {
      if (var.modifier == GLSLVar::UNIFORM)
        m_uniform_var_names.insert(var.name);
      else if (stage.stage_type() == GL_VERTEX_SHADER &&
               var.modifier == GLSLVar::IN)
        m_attribute_var_names.insert(var.name);
    }
    stage.delete_stage();
  }

  glLinkProgram(m_id);
  gl_error_check("glLinkProgram");
  info_log();
  bind();
  for (const auto& var : m_attribute_var_names) add_attribute(var);
  for (const auto& var : m_uniform_var_names) add_uniform(var);
}

//------------------------------------------------------------------------------

void Shader::delete_shader() {
  if (m_id != 0) {
    glDeleteProgram(m_id);
    gl_error_check("glDeleteProgram");
  }

  m_id = 0;
}

//------------------------------------------------------------------------------

void Shader::bind() {
  glUseProgram(m_id);
  gl_error_check("glUseProgram");
}

//------------------------------------------------------------------------------

void Shader::unbind() {
  glUseProgram(0);
  gl_error_check("glUseProgram");
}

//------------------------------------------------------------------------------

void Shader::add_uniform(const std::string& uniformVarName) {
  m_uniform_locations.insert(std::pair<std::string, GLint>(
      uniformVarName, glGetUniformLocation(m_id, uniformVarName.c_str())));

  //------------------------------------------------------------------------------
}

//------------------------------------------------------------------------------

void Shader::add_attribute(const std::string& attributeVarName) {
  m_attribute_locations.insert(std::pair<std::string, GLint>(
      attributeVarName, glGetAttribLocation(m_id, attributeVarName.c_str())));
}

//------------------------------------------------------------------------------

GLint Shader::uniform(const std::string& uniformVarName) {
  return m_uniform_locations[uniformVarName];
}

//------------------------------------------------------------------------------

GLint Shader::attribute(const std::string& attributeVarName) {
  return m_attribute_locations[attributeVarName];
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
  glProgramUniform2fv(m_id, m_uniform_locations[name], 1,
                      glm::value_ptr(value));
  gl_error_check("glProgramUniform2fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  glProgramUniform3fv(m_id, m_uniform_locations[name], 1,
                      glm::value_ptr(value));
  gl_error_check("glProgramUniform3fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
  glProgramUniform4fv(m_id, m_uniform_locations[name], 1,
                      glm::value_ptr(value));
  gl_error_check("glProgramUniform4fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat2x2& value) {
  glProgramUniformMatrix2fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                            glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix2fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat3x3& value) {
  glProgramUniformMatrix3fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                            glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix3fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat4x4& value) {
  glProgramUniformMatrix4fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                            glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix4fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, float value) {
  glProgramUniform1f(m_id, m_uniform_locations[name], value);
  gl_error_check("glProgramUniform1f");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, int value) {
  glProgramUniform1i(m_id, m_uniform_locations[name], value);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, unsigned int value) {
  glProgramUniform1ui(m_id, m_uniform_locations[name], value);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<float> value) {
  glProgramUniform1f(m_id, m_uniform_locations[name], value[0]);
  gl_error_check("glProgramUniform1f");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<int32_t> value) {
  glProgramUniform1i(m_id, m_uniform_locations[name], value[0]);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<uint32_t> value) {
  glProgramUniform1ui(m_id, m_uniform_locations[name], value[0]);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<float>& value) {
  glProgramUniform2fv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const float*>(&value));
  gl_error_check("glProgramUniform2fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<float>& value) {
  glProgramUniform3fv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const float*>(&value));
  gl_error_check("glProgramUniform3fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<float>& value) {
  glProgramUniform4fv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const float*>(&value));
  gl_error_check("glProgramUniform4fv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<int32_t>& value) {
  glProgramUniform2iv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const int32_t*>(&value));
  gl_error_check("glProgramUniform2iv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<int32_t>& value) {
  glProgramUniform3iv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const int32_t*>(&value));
  gl_error_check("glProgramUniform3iv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<int32_t>& value) {
  glProgramUniform4iv(m_id, m_uniform_locations[name], 1,
                      reinterpret_cast<const int32_t*>(&value));
  gl_error_check("glProgramUniform4iv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<uint32_t>& value) {
  glProgramUniform2uiv(m_id, m_uniform_locations[name], 1,
                       reinterpret_cast<const uint32_t*>(&value));
  gl_error_check("glProgramUniform2uiv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<uint32_t>& value) {
  glProgramUniform3uiv(m_id, m_uniform_locations[name], 1,
                       reinterpret_cast<const uint32_t*>(&value));
  gl_error_check("glProgramUniform3uiv");
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<uint32_t>& value) {
  glProgramUniform4uiv(m_id, m_uniform_locations[name], 1,
                       reinterpret_cast<const uint32_t*>(&value));
  gl_error_check("glProgramUniform4uiv");
}

//------------------------------------------------------------------------------

void Shader::info_log() {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;

  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
  gl_error_check("glGetProgramiv - GL_INFO_LOG_LENGTH");
  auto infoLog = new char[infoLogLength];
  glGetProgramInfoLog(m_id, infoLogLength, &charsWritten, infoLog);
  gl_error_check("glGetProgramInfoLog");

  delete[] infoLog;
}

//==============================================================================
}  // namespace Yavin
//==============================================================================
