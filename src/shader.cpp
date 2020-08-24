#include <yavin/glfunctions.h>
#include <yavin/shader.h>
//==============================================================================
namespace yavin {
//==============================================================================
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
void shader::bind() const {
  gl::use_program(m_id);
}
//------------------------------------------------------------------------------
void shader::unbind() const {
  gl::use_program(0);
}
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
void shader::set_uniform(const std::string& name, GLboolean value) {
  gl::program_uniform_1i(m_id, m_uniform_locations.at(name), value);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLfloat value) {
  gl::program_uniform_1f(m_id, m_uniform_locations.at(name), value);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLfloat x, GLfloat y) {
  gl::program_uniform_2f(m_id, m_uniform_locations.at(name), x, y);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLfloat x, GLfloat y,
                         GLfloat z) {
  gl::program_uniform_3f(m_id, m_uniform_locations.at(name), x, y, z);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLfloat x, GLfloat y,
                         GLfloat z, GLfloat w) {
  gl::program_uniform_4f(m_id, m_uniform_locations.at(name), x, y, z, w);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&            name,
                         std::array<GLfloat, 2> const& data) {
  gl::program_uniform_2fv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&            name,
                         std::array<GLfloat, 3> const& data) {
  gl::program_uniform_3fv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&            name,
                         std::array<GLfloat, 4> const& data) {
  gl::program_uniform_4fv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLint value) {
  gl::program_uniform_1i(m_id, m_uniform_locations.at(name), value);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLint x, GLint y) {
  gl::program_uniform_2i(m_id, m_uniform_locations.at(name), x, y);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLint x, GLint y, GLint z) {
  gl::program_uniform_3i(m_id, m_uniform_locations.at(name), x, y, z);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLint x, GLint y, GLint z,
                         GLint w) {
  gl::program_uniform_4i(m_id, m_uniform_locations.at(name), x, y, z, w);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&          name,
                         std::array<GLint, 2> const& data) {
  gl::program_uniform_2iv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&          name,
                         std::array<GLint, 3> const& data) {
  gl::program_uniform_3iv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&          name,
                         std::array<GLint, 4> const& data) {
  gl::program_uniform_4iv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLuint value) {
  gl::program_uniform_1ui(m_id, m_uniform_locations.at(name), value);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLuint x, GLuint y) {
  gl::program_uniform_2ui(m_id, m_uniform_locations.at(name), x, y);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLuint x, GLuint y,
                         GLuint z) {
  gl::program_uniform_3ui(m_id, m_uniform_locations.at(name), x, y, z);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string& name, GLuint x, GLuint y, GLuint z,
                         GLuint w) {
  gl::program_uniform_4ui(m_id, m_uniform_locations.at(name), x, y, z, w);
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&           name,
                         std::array<GLuint, 2> const& data) {
  gl::program_uniform_2uiv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&           name,
                         std::array<GLuint, 3> const& data) {
  gl::program_uniform_3uiv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform(const std::string&           name,
                         std::array<GLuint, 4> const& data) {
  gl::program_uniform_4uiv(m_id, m_uniform_locations.at(name), 1, data.data());
}
//------------------------------------------------------------------------------
void shader::set_uniform_vec2(const std::string& name, GLfloat const* data) {
  gl::program_uniform_2fv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec2(const std::string& name, GLint const* data) {
  gl::program_uniform_2iv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec2(const std::string& name, GLuint const* data) {
  gl::program_uniform_2uiv(m_id, m_uniform_locations.at(name), 1, data);
}
//------------------------------------------------------------------------------
void shader::set_uniform_vec3(const std::string& name, GLfloat const* data) {
  gl::program_uniform_3fv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec3(const std::string& name, GLint const* data) {
  gl::program_uniform_3iv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec3(const std::string& name, GLuint const* data) {
  gl::program_uniform_3uiv(m_id, m_uniform_locations.at(name), 1, data);
}
//------------------------------------------------------------------------------
void shader::set_uniform_vec4(const std::string& name, GLfloat const* data) {
  gl::program_uniform_4fv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec4(const std::string& name, GLint const* data) {
  gl::program_uniform_4iv(m_id, m_uniform_locations.at(name), 1, data);
}
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void shader::set_uniform_vec4(const std::string& name, GLuint const* data) {
  gl::program_uniform_4uiv(m_id, m_uniform_locations.at(name), 1, data);
}
//------------------------------------------------------------------------------
void shader::set_uniform_mat2(const std::string& name, GLfloat const* data) {
  gl::program_uniform_matrix_2fv(m_id, m_uniform_locations.at(name), 1, false,
                                 data);
}
//------------------------------------------------------------------------------
void shader::set_uniform_mat3(const std::string& name, GLfloat const* data) {
  gl::program_uniform_matrix_3fv(m_id, m_uniform_locations.at(name), 1, false,
                                 data);
}
//------------------------------------------------------------------------------
void shader::set_uniform_mat4(const std::string& name, GLfloat const* data) {
  gl::program_uniform_matrix_4fv(m_id, m_uniform_locations.at(name), 1, false,
                                 data);
}
//------------------------------------------------------------------------------
std::optional<std::string> shader::info_log() {
  GLint   info_log_length = 0;
  GLsizei chars_written   = 0;

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
