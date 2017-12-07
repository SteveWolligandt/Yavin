#include "Shader.h"

#include "gl_includes.h"

namespace Yavin {

Shader::Shader() {
  m_id = glCreateProgram();
  gl_error_check("glCreateProgram");
}

Shader::~Shader() { glDeleteProgram(m_id); }

template <typename T, typename... Args>
void Shader::add_shader_stage(Args... args) {
  m_shader_stages.push_back(T(args...));
  glAttachShader(m_id, m_shader_stages.back().id());
  gl_error_check("glAttachShader - " + m_shader_stages.back().stage());
  for (const auto& stage : m_shader_stages)
    for (const auto& var : stage.glsl_vars()) {
      if (var.modifier == GLSLVar::UNIFORM)
        m_uniform_var_names.insert(var.name);
      else if (std::is_same<VertexShader, T>::value && var.modifier == GLSLVar::IN)
        m_attribute_var_names.insert(var.name);
    }
}

template void Shader::add_shader_stage<VertexShader, char const*>(char const*);
template void Shader::add_shader_stage<FragmentShader, char const*>(char const*);
template void Shader::add_shader_stage<GeometryShader, char const*>(char const*);
template void Shader::add_shader_stage<TesselationEvaluationShader, char const*>(char const*);
template void Shader::add_shader_stage<TesselationControlShader, char const*>(char const*);
template void Shader::add_shader_stage<ComputeShader, char const*>(char const*);

void Shader::create() {
  glLinkProgram(m_id);
  gl_error_check("glLinkProgram");
  info_log();
  bind();
  for (const auto& var : m_attribute_var_names) add_attribute(var);
  for (const auto& var : m_uniform_var_names) add_uniform(var);
}

void Shader::bind() {
  glUseProgram(m_id);
  gl_error_check("glUseProgram");
}

void Shader::unbind() {
  glUseProgram(0);
  gl_error_check("glUseProgram");
}

void Shader::add_uniform(const std::string& uniformVarName) {
  m_uniform_locations.insert(
      std::pair<std::string, GLint>(uniformVarName, glGetUniformLocation(m_id, uniformVarName.c_str())));
}

void Shader::add_attribute(const std::string& attributeVarName) {
  m_attribute_locations.insert(
      std::pair<std::string, GLint>(attributeVarName, glGetAttribLocation(m_id, attributeVarName.c_str())));
}

GLint Shader::uniform(const std::string& uniformVarName) { return m_uniform_locations[uniformVarName]; }

GLint Shader::attribute(const std::string& attributeVarName) { return m_attribute_locations[attributeVarName]; }

void Shader::set_uniform(const std::string& name, float value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniform1f(m_id, m_uniform_locations[name], value);
  gl_error_check("glProgramUniform1f");
}

void Shader::set_uniform(const std::string& name, int value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniform1i(m_id, m_uniform_locations[name], value);
  gl_error_check("glProgramUniform1i");
}

void Shader::set_uniform(const std::string& name, const glm::mat4x4& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniformMatrix4fv(m_id, m_uniform_locations[name], 1, GL_FALSE, glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix4fv");
}

void Shader::set_uniform(const std::string& name, const glm::mat3x3& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniformMatrix3fv(m_id, m_uniform_locations[name], 1, GL_FALSE, glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix3fv");
}

void Shader::set_uniform(const std::string& name, const glm::mat2x2& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniformMatrix2fv(m_id, m_uniform_locations[name], 1, GL_FALSE, glm::value_ptr(value));
  gl_error_check("glProgramUniformMatrix2fv");
}

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniform4fv(m_id, m_uniform_locations[name], 1, glm::value_ptr(value));
  gl_error_check("glProgramUniform4fv");
}

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniform3fv(m_id, m_uniform_locations[name], 1, glm::value_ptr(value));
  gl_error_check("glProgramUniform3fv");
}

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
  assert(m_uniform_locations.find(name) != m_uniform_locations.end());
  glProgramUniform2fv(m_id, m_uniform_locations[name], 1, glm::value_ptr(value));
  gl_error_check("glProgramUniform2fv");
}

// Print information about the linking step
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

}  // namespace Yavin
