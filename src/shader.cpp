#include <yavin/shader.h>

#include <yavin/glfunctions.h>

//==============================================================================
namespace yavin {
//==============================================================================

template void shader::add_stage<vertexshader, char const*>(char const*);
template void shader::add_stage<fragmentshader, char const*>(char const*);
template void shader::add_stage<geometryshader, char const*>(char const*);
template void shader::add_stage<tesselationevaluationshader, char const*>(
    char const*);
template void shader::add_stage<tesselationcontrolshader, char const*>(
    char const*);
template void shader::add_stage<computeshader, char const*>(char const*);

//------------------------------------------------------------------------------

void shader::create() {
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
  if (auto log = info_log(); log) { throw std::runtime_error{std::move(*log)}; }
  bind();
  for (const auto& var : m_attribute_var_names) add_attribute(var);
  for (const auto& var : m_uniform_var_names) add_uniform(var);
}

//------------------------------------------------------------------------------

void shader::delete_shader() {
  if (m_id != 0) gl::delete_program(m_id);
  m_id = 0;
}

//------------------------------------------------------------------------------

void shader::bind() { gl::use_program(m_id); }

//------------------------------------------------------------------------------

void shader::unbind() { gl::use_program(0); }

//------------------------------------------------------------------------------

void shader::add_uniform(const std::string& uniformVarName) {
  m_uniform_locations.insert(std::pair<std::string, GLint>(
      uniformVarName, gl::get_uniform_location(m_id, uniformVarName.c_str())));
}

//------------------------------------------------------------------------------

void shader::add_attribute(const std::string& attributeVarName) {
  m_attribute_locations.insert(std::pair<std::string, GLint>(
      attributeVarName,
      gl::get_uniform_location(m_id, attributeVarName.c_str())));
}

//------------------------------------------------------------------------------

GLint shader::uniform(const std::string& uniformVarName) {
  return m_uniform_locations[uniformVarName];
}

//------------------------------------------------------------------------------

GLint shader::attribute(const std::string& attributeVarName) {
  return m_attribute_locations[attributeVarName];
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::ivec2& value) {
  gl::program_uniform_2iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::ivec3& value) {
  gl::program_uniform_3iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::ivec4& value) {
  gl::program_uniform_4iv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::vec2& value) {
  gl::program_uniform_2fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::vec3& value) {
  gl::program_uniform_3fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::vec4& value) {
  gl::program_uniform_4fv(m_id, m_uniform_locations[name], 1,
                          glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::mat2x2& value) {
  gl::program_uniform_matrix_2fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::mat3x3& value) {
  gl::program_uniform_matrix_3fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const glm::mat4x4& value) {
  gl::program_uniform_matrix_4fv(m_id, m_uniform_locations[name], 1, GL_FALSE,
                                 glm::value_ptr(value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, float value) {
  gl::program_uniform_1f(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, float x, float y) {
  gl::program_uniform_2f(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, float x, float y, float z) {
  gl::program_uniform_3f(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, float x, float y, float z,
                         float w) {
  gl::program_uniform_4f(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, int value) {
  gl::program_uniform_1i(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, int32_t x, int32_t y) {
  gl::program_uniform_2i(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, int32_t x, int32_t y,
                         int32_t z) {
  gl::program_uniform_3i(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, int32_t x, int32_t y,
                         int32_t z, int32_t w) {
  gl::program_uniform_4i(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, unsigned int value) {
  gl::program_uniform_1ui(m_id, m_uniform_locations[name], value);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, uint32_t x, uint32_t y) {
  gl::program_uniform_2ui(m_id, m_uniform_locations[name], x, y);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, uint32_t x, uint32_t y,
                         uint32_t z) {
  gl::program_uniform_3ui(m_id, m_uniform_locations[name], x, y, z);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, uint32_t x, uint32_t y,
                         uint32_t z, uint32_t w) {
  gl::program_uniform_4ui(m_id, m_uniform_locations[name], x, y, z, w);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, Scalar<float> value) {
  gl::program_uniform_1f(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, Scalar<int32_t> value) {
  gl::program_uniform_1i(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, Scalar<uint32_t> value) {
  gl::program_uniform_1ui(m_id, m_uniform_locations[name], value[0]);
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec2<float>& value) {
  gl::program_uniform_2fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec3<float>& value) {
  gl::program_uniform_3fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec4<float>& value) {
  gl::program_uniform_4fv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const float*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec2<int32_t>& value) {
  gl::program_uniform_2iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec3<int32_t>& value) {
  gl::program_uniform_3iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec4<int32_t>& value) {
  gl::program_uniform_4iv(m_id, m_uniform_locations[name], 1,
                          reinterpret_cast<const int32_t*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec2<uint32_t>& value) {
  gl::program_uniform_2uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec3<uint32_t>& value) {
  gl::program_uniform_3uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

void shader::set_uniform(const std::string& name, const Vec4<uint32_t>& value) {
  gl::program_uniform_4uiv(m_id, m_uniform_locations[name], 1,
                           reinterpret_cast<const uint32_t*>(&value));
}

//------------------------------------------------------------------------------

std::optional<std::string> shader::info_log() {
  GLint   info_log_length = 0;
  GLsizei chars_written  = 0;

  gl::get_program_iv(m_id, GL_INFO_LOG_LENGTH, &info_log_length);
  if (info_log_length == 0) { return {}; }
  std::string l(info_log_length, ' ');
  gl::get_program_info_log(m_id, info_log_length, &chars_written,
                           const_cast<GLchar*>(l.c_str()));

  return l;
}

//==============================================================================
}  // namespace yavin
//==============================================================================
