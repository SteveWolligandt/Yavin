#include "Shader.h"

#include "gl_functions.h"

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
  m_id = gl::create_program();

  for (auto& stage : m_shader_stages) {
    stage.compile();
    gl::attach_shader(m_id, stage.id());
    for (const auto& var : stage.glsl_vars()) {
      if (var.modifier == GLSLVar::UNIFORM)
        m_uniform_var_names.insert(var.name);
      else if (stage.stage_type() == GL_VERTEX_SHADER &&
               var.modifier == GLSLVar::IN)
        m_attribute_var_names.insert(var.name);
    }
    stage.delete_stage();
  }

  gl::link_program(m_id);
  info_log();
  bind();
  for (const auto& var : m_attribute_var_names) add_attribute(var);
  for (const auto& var : m_uniform_var_names) add_uniform(var);
}

//------------------------------------------------------------------------------

void Shader::delete_shader() {
  if (m_id != 0) gl::delete_program(m_id);
  m_id = 0;
}

//------------------------------------------------------------------------------

void Shader::bind() { gl::use_program(m_id); }

//------------------------------------------------------------------------------

void Shader::unbind() { gl::use_program(0); }

//------------------------------------------------------------------------------

void Shader::add_uniform(const std::string& uniformVarName) {
  m_uniform_locations.insert(std::pair<std::string, GLint>(
      uniformVarName, gl::get_uniform_location(m_id, uniformVarName.c_str())));
}

//------------------------------------------------------------------------------

void Shader::add_attribute(const std::string& attributeVarName) {
  m_attribute_locations.insert(std::pair<std::string, GLint>(
      attributeVarName,
      gl::get_uniform_location(m_id, attributeVarName.c_str())));
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

void Shader::set_uniform(const std::string& name, const glm::ivec2& value) {
  gl::program_uniform_2iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::ivec3& value) {
  gl::program_uniform_3iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::ivec4& value) {
  gl::program_uniform_4iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec2& value) {
  gl::program_uniform_2fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec3& value) {
  gl::program_uniform_3fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::vec4& value) {
  gl::program_uniform_4fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat2x2& value) {
  gl::program_uniform_matrix_2fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat3x3& value) {
  gl::program_uniform_matrix_3fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const glm::mat4x4& value) {
  gl::program_uniform_matrix_4fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, float value) {
  gl::program_uniform_1f(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, float x, float y) {
  gl::program_uniform_2f(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, float x, float y, float z) {
  gl::program_uniform_3f(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, float x, float y, float z,
                         float w) {
  gl::program_uniform_4f(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, int value) {
  gl::program_uniform_1i(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, int32_t x, int32_t y) {
  gl::program_uniform_2i(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, int32_t x, int32_t y,
                         int32_t z) {
  gl::program_uniform_3i(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, int32_t x, int32_t y,
                         int32_t z, int32_t w) {
  gl::program_uniform_4i(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, unsigned int value) {
  gl::program_uniform_1ui(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, uint32_t x, uint32_t y) {
  gl::program_uniform_2ui(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, uint32_t x, uint32_t y,
                         uint32_t z) {
  gl::program_uniform_3ui(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, uint32_t x, uint32_t y,
                         uint32_t z, uint32_t w) {
  gl::program_uniform_4ui(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<float> value) {
  gl::program_uniform_1f(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<int32_t> value) {
  gl::program_uniform_1i(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, Scalar<uint32_t> value) {
  gl::program_uniform_1ui(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<float>& value) {
  gl::program_uniform_2fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<float>& value) {
  gl::program_uniform_3fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<float>& value) {
  gl::program_uniform_4fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<int32_t>& value) {
  gl::program_uniform_2iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<int32_t>& value) {
  gl::program_uniform_3iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<int32_t>& value) {
  gl::program_uniform_4iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec2<uint32_t>& value) {
  gl::program_uniform_2uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec3<uint32_t>& value) {
  gl::program_uniform_3uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::set_uniform(const std::string& name, const Vec4<uint32_t>& value) {
  gl::program_uniform_4uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

void Shader::info_log() {
  GLint   infoLogLength = 0;
  GLsizei charsWritten  = 0;
  auto    infoLog       = new char[infoLogLength];

  gl::get_program_iv(m_id, GL_INFO_LOG_LENGTH, &infoLogLength);
  gl::get_program_info_log(m_id, infoLogLength, &charsWritten, infoLog);

  delete[] infoLog;
}

//==============================================================================
}  // namespace Yavin
//==============================================================================
