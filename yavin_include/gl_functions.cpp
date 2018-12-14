#include "gl_functions.h"
#include "to_string.h"

//==============================================================================
namespace Yavin {
//==============================================================================
// MISC
//==============================================================================

void gl::enable(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glEnable(" << to_string(cap) << ")\n";
  glEnable(cap);
  gl_error_check("glEnable");
}

//------------------------------------------------------------------------------

GLboolean gl::is_enabled(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glIsEnabled(" << to_string(cap) << ")\n";
  auto result = glIsEnabled(cap);
  gl_error_check("glIsEnabled");
  return result;
}

//------------------------------------------------------------------------------

void gl::disable(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDisable(" << to_string(cap) << ")\n";
  glDisable(cap);
  gl_error_check("glDisable");
}

//------------------------------------------------------------------------------

const GLubyte* gl::get_string(GLenum name) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetString\n";
  auto result = glGetString(name);
  gl_error_check("glGetString");
  return result;
}  // namespace Yavin

//------------------------------------------------------------------------------

GLenum gl::get_error() { return glGetError(); }

//==============================================================================
// BACKBUFFER RELATED
//==============================================================================

void gl::clear(GLbitfield mask) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glClear\n";
  glClear(mask);
  gl_error_check("glClear");
}

//------------------------------------------------------------------------------

void gl::clear_color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glClearColor(" << red << ", " << green << ", " << blue << ", "
         << alpha << ")\n";
  glClearColor(red, green, blue, alpha);
  gl_error_check("glClearColor");
}

//------------------------------------------------------------------------------

void gl::viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glViewport(" << x << ", " << y << ", " << width << ", " << height
         << ")\n";
  glViewport(x, y, width, height);
  gl_error_check("glViewport");
}

//------------------------------------------------------------------------------

void gl::viewport(const glm::vec4& vp) { viewport(vp[1], vp[1], vp[2], vp[3]); }

//------------------------------------------------------------------------------

void gl::flush() {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glFlush\n";
  glFlush();
  gl_error_check("glFlush");
}

//------------------------------------------------------------------------------

void gl::depth_mask(GLboolean flag) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDepthMask(" << flag << ")\n";
  glDepthMask(flag);
  gl_error_check("glDepthMask");
}

//------------------------------------------------------------------------------

void gl::blend_func(GLenum sfactor, GLenum dfactor) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glBlendFunc(" << sfactor << ", " << dfactor << ")\n";
  glBlendFunc(sfactor, dfactor);
  gl_error_check("glBlendFunc");
}

//------------------------------------------------------------------------------

void gl::blend_funci(GLuint buf, GLenum sfactor, GLenum dfactor) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glBlendFunci(" << buf << ", " << sfactor << ", " << dfactor
         << ")\n";
  glBlendFunci(buf, sfactor, dfactor);
  gl_error_check("glBlendFunci");
}

//==============================================================================
// VERTEXBUFFER RELATED
//==============================================================================

void gl::enable_vertex_attrib_array(GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glEnableVertexAttribArray(" << index << ")\n";
  glEnableVertexAttribArray(index);
  gl_error_check("glEnableVertexAttribArray");
}

//------------------------------------------------------------------------------

void gl::disable_vertex_attrib_array(GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDisableVertexAttribArray(" << index << ")\n";
  glDisableVertexAttribArray(index);
  gl_error_check("glDisableVertexAttribArray");
}

//------------------------------------------------------------------------------

void gl::enable_vertex_attrib_array(GLuint vaobj, GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glEnableVertexArrayAttrib(" << vaobj << ", " << index << ")\n";
  glEnableVertexArrayAttrib(vaobj, index);
  gl_error_check("glEnableVertexArrayAttrib");
}

//------------------------------------------------------------------------------

// gl::void disable_vertex_attrib_array(GLuint vaobj, GLuint index) {
//  std::lock_guard lock(detail::mutex::gl_call);
//  if(verbose)*out<<"glDisableVertexArrayAttrib\n";
//  glDisableVertexArrayAttrib(vaobj, index);
//  gl_error_check("glDisableVertexArrayAttrib");
//}

//------------------------------------------------------------------------------

void gl::vertex_attrib_pointer(GLuint index, GLint size, GLenum type,
                               GLboolean normalized, GLsizei stride,
                               const GLvoid* pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glVertexAttribPointer\n";
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  gl_error_check("glVertexAttribPointer");
}

//------------------------------------------------------------------------------

void gl::vertex_attrib_i_pointer(GLuint index, GLint size, GLenum type,
                                 GLsizei stride, const GLvoid* pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glVertexAttribIPointer\n";
  glVertexAttribIPointer(index, size, type, stride, pointer);
  gl_error_check("glVertexAttribIPointer");
}

//------------------------------------------------------------------------------

void gl::vertex_attrib_l_pointer(GLuint index, GLint size, GLenum type,
                                 GLsizei stride, const GLvoid* pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glVertexAttribLPointer\n";
  glVertexAttribLPointer(index, size, type, stride, pointer);
  gl_error_check("glVertexAttribLPointer");
}

//==============================================================================
// VERTEXARRAY RELATED
//==============================================================================

void gl::create_vertex_arrays(GLsizei n, GLuint* arrays) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateVertexArrays(" << n << ", " << arrays << ")";
  glCreateVertexArrays(n, arrays);
  if (verbose) {
    *out << " = [ ";
    for (GLsizei i = 0; i < n; ++i) *out << arrays[i] << ' ';
    *out << "]\n";
  }
  gl_error_check("glCreateVertexArrays");
}

//------------------------------------------------------------------------------

void gl::delete_vertex_arrays(GLsizei n, GLuint* arrays) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) {
    *out << "glDeleteVertexArrays(" << n << ", " << arrays << ") = [ ";
    for (GLsizei i = 0; i < n; ++i) *out << arrays[i] << ' ';
    *out << "]\n";
  }
  glDeleteVertexArrays(n, arrays);
  gl_error_check("glDeleteVertexArrays");
}

//------------------------------------------------------------------------------

void gl::bind_vertex_array(GLuint array) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glBindVertexArray(" << array << ")\n";
  glBindVertexArray(array);
  gl_error_check("glBindVertexArray");
}

//------------------------------------------------------------------------------

void gl::draw_elements(GLenum mode, GLsizei count, GLenum type,
                       const GLvoid* indices) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDrawElements\n";
  glDrawElements(mode, count, type, indices);
  gl_error_check("glDrawElements");
}

//==============================================================================
// BUFFER RELATED
//==============================================================================

void gl::bind_buffer(GLenum target, GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glBindBuffer(" << to_string(target) << ", " << buffer << ")\n";
  glBindBuffer(target, buffer);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

void gl::bind_buffer_base(GLenum target, GLuint index, GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glBindBufferBase(" << to_string(target) << ", " << index << ", "
         << buffer << ")\n";
  glBindBufferBase(target, index, buffer);
  gl_error_check("glBindBufferBase");
}

//------------------------------------------------------------------------------

void gl::create_buffers(GLsizei n, GLuint* buffers) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateBuffers(" << n << ", " << buffers << ")";
  glCreateBuffers(1, buffers);
  if (verbose) {
    *out << " = [ ";
    for (GLsizei i = 0; i < n; ++i) *out << buffers[i] << ' ';
    *out << "]\n";
  }
  gl_error_check("glCreateBuffers");
}

//------------------------------------------------------------------------------

void gl::delete_buffers(GLsizei n, GLuint* buffers) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) {
    *out << "glDeleteBuffers(" << n << ", " << buffers << ") = [ ";
    for (GLsizei i = 0; i < n; ++i) *out << buffers[i] << ' ';
    *out << "]\n";
  }
  glDeleteBuffers(1, buffers);
  gl_error_check("glCreateBuffers");
}

//------------------------------------------------------------------------------

void gl::copy_named_buffer_sub_data(GLuint readBuffer, GLuint writeBuffer,
                                    GLintptr readOffset, GLintptr writeOffset,
                                    GLsizei size) {
  std::lock_guard lock(detail::mutex::buffer);
  if (verbose) *out << "glCopyNamedBufferSubData\n";
  glCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset,
                           size);
  gl_error_check("glCopyNamedBufferSubData");
}

//------------------------------------------------------------------------------

void* gl::map_named_buffer(GLuint buffer, GLenum access) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glMapNamedBuffer(" << buffer << ", " << to_string(access) << ")\n";
  auto result = glMapNamedBuffer(buffer, access);
  gl_error_check("glMapNamedBuffer");
  return result;
}

//------------------------------------------------------------------------------

void* gl::map_named_buffer_range(GLuint buffer, GLintptr offset, GLsizei length,
                                 GLbitfield access) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glMapNamedBufferRange(" << buffer << ", " << offset << ", "
         << length << ", " << map_access_to_string(access) << ")\n";
  auto result = glMapNamedBufferRange(buffer, offset, length, access);
  gl_error_check("glMapNamedBufferRange");
  return result;
}

//------------------------------------------------------------------------------

void gl::named_buffer_data(GLuint buffer, GLsizei size, const void* data,
                           GLenum usage) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glNamedBufferData(" << buffer << ", " << size << ", " << data
         << ", " << to_string(usage) << ")\n";
  glNamedBufferData(buffer, size, data, usage);
  gl_error_check("glNamedBufferData");
}

//------------------------------------------------------------------------------

GLboolean gl::unmap_named_buffer(GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glUnmapNamedBuffer(" << buffer << ")\n";
  auto result = glUnmapNamedBuffer(buffer);
  gl_error_check("glUnmapNamedBuffer");
  return result;
}

//------------------------------------------------------------------------------

void gl::named_buffer_sub_data(GLuint buffer, GLintptr offset, GLsizei size,
                               const void* data) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glNamedBufferSubData(" << buffer << ", " << offset << ", " << size
         << ", " << data << ")\n";
  glNamedBufferSubData(buffer, offset, size, data);
  gl_error_check("glNamedBufferSubData");
}

//==============================================================================
// SHADER RELATED
//==============================================================================

GLuint gl::create_program() {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateProgram\n";
  auto result = glCreateProgram();
  gl_error_check("glCreateProgram");
  return result;
}

//------------------------------------------------------------------------------

void gl::attach_shader(GLuint program, GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glAttachShader\n";
  glAttachShader(program, shader);
  gl_error_check("glAttachShader");
}

//------------------------------------------------------------------------------

void gl::link_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glLinkProgram\n";
  glLinkProgram(program);
  gl_error_check("glLinkProgram");
}

//------------------------------------------------------------------------------

void gl::delete_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDeleteProgram\n";
  glDeleteProgram(program);
  gl_error_check("glDeleteProgram");
}

//------------------------------------------------------------------------------

void gl::use_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glUseProgram(" << program << ")\n";
  glUseProgram(program);
  gl_error_check("glUseProgram");
}

//------------------------------------------------------------------------------

GLuint gl::create_shader(GLenum shaderType) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateShader\n";
  auto result = glCreateShader(shaderType);
  gl_error_check("glCreateShader");
  return result;
}

//------------------------------------------------------------------------------

void gl::shader_source(GLuint shader, GLsizei count, const GLchar** string,
                       const GLint* length) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glShaderSource\n";
  glShaderSource(shader, count, string, length);
  gl_error_check("glShaderSource");
}

//------------------------------------------------------------------------------

void gl::compile_shader(GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCompileShader\n";
  glCompileShader(shader);
  gl_error_check("glCompileShader");
}

//------------------------------------------------------------------------------

void gl::delete_shader(GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDeleteShader\n";
  glDeleteShader(shader);
  gl_error_check("glDeleteShader");
}

//------------------------------------------------------------------------------

void gl::get_shader_iv(GLuint shader, GLenum pname, GLint* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetShaderiv\n";
  glGetShaderiv(shader, pname, params);
  gl_error_check("glGetShaderiv");
}

//------------------------------------------------------------------------------

GLint gl::get_shader_info_log_length(GLuint shader) {
  GLint info_log_length;
  get_shader_iv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
  return info_log_length;
}

//------------------------------------------------------------------------------

void gl::get_shader_info_log(GLuint shader, GLsizei maxLength, GLsizei* length,
                             GLchar* infoLog) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetShaderInfoLog\n";
  glGetShaderInfoLog(shader, maxLength, length, infoLog);
  gl_error_check("glGetShaderInfoLog");
}

//------------------------------------------------------------------------------

std::string gl::get_shader_info_log(GLuint shader, GLsizei maxLength) {
  std::vector<char> buffer(maxLength);
  if (verbose) *out << "data\n";
  get_shader_info_log(shader, maxLength, nullptr, buffer.data());
  std::string log(buffer.begin(), buffer.end());
  return log;
}

//------------------------------------------------------------------------------

std::string gl::get_shader_info_log(GLuint shader) {
  return get_shader_info_log(shader, get_shader_info_log_length(shader));
}

//------------------------------------------------------------------------------

void gl::program_uniform_1f(GLuint program, GLint location, GLfloat v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1f\n";
  glProgramUniform1f(program, location, v0);
  gl_error_check("glProgramUniform1f");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2f(GLuint program, GLint location, GLfloat v0,
                            GLfloat v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2f\n";
  glProgramUniform2f(program, location, v0, v1);
  gl_error_check("glProgramUniform2f");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3f(GLuint program, GLint location, GLfloat v0,
                            GLfloat v1, GLfloat v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3f\n";
  glProgramUniform3f(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3f");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4f(GLuint program, GLint location, GLfloat v0,
                            GLfloat v1, GLfloat v2, GLfloat v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4f\n";
  glProgramUniform4f(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4f");
}

//------------------------------------------------------------------------------

void gl::program_uniform_1i(GLuint program, GLint location, GLint v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1i\n";
  glProgramUniform1i(program, location, v0);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2i(GLuint program, GLint location, GLint v0,
                            GLint v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2i\n";
  glProgramUniform2i(program, location, v0, v1);
  gl_error_check("glProgramUniform2i");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3i(GLuint program, GLint location, GLint v0, GLint v1,
                            GLint v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3i\n";
  glProgramUniform3i(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3i");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4i(GLuint program, GLint location, GLint v0, GLint v1,
                            GLint v2, GLint v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4i\n";
  glProgramUniform4i(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4i");
}

//------------------------------------------------------------------------------

void gl::program_uniform_1ui(GLuint program, GLint location, GLuint v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1ui\n";
  glProgramUniform1ui(program, location, v0);
  gl_error_check("glProgramUniform1ui");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2ui(GLuint program, GLint location, GLuint v0,
                             GLuint v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2ui\n";
  glProgramUniform2ui(program, location, v0, v1);
  gl_error_check("glProgramUniform2ui");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3ui(GLuint program, GLint location, GLuint v0,
                             GLuint v1, GLuint v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3ui\n";
  glProgramUniform3ui(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3ui");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4ui(GLuint program, GLint location, GLuint v0,
                             GLuint v1, GLuint v2, GLuint v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4ui\n";
  glProgramUniform4ui(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4ui");
}

//------------------------------------------------------------------------------

void gl::program_uniform_1fv(GLuint program, GLint location, GLsizei count,
                             const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1fv\n";
  glProgramUniform1fv(program, location, count, value);
  gl_error_check("glProgramUniform1fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2fv(GLuint program, GLint location, GLsizei count,
                             const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2fv\n";
  glProgramUniform2fv(program, location, count, value);
  gl_error_check("glProgramUniform2fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3fv(GLuint program, GLint location, GLsizei count,
                             const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3fv\n";
  glProgramUniform3fv(program, location, count, value);
  gl_error_check("glProgramUniform3fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4fv(GLuint program, GLint location, GLsizei count,
                             const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4fv\n";
  glProgramUniform4fv(program, location, count, value);
  gl_error_check("glProgramUniform4fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_1iv(GLuint program, GLint location, GLsizei count,
                             const GLint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1iv\n";
  glProgramUniform1iv(program, location, count, value);
  gl_error_check("glProgramUniform1iv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2iv(GLuint program, GLint location, GLsizei count,
                             const GLint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2iv\n";
  glProgramUniform2iv(program, location, count, value);
  gl_error_check("glProgramUniform2iv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3iv(GLuint program, GLint location, GLsizei count,
                             const GLint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3iv\n";
  glProgramUniform3iv(program, location, count, value);
  gl_error_check("glProgramUniform3iv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4iv(GLuint program, GLint location, GLsizei count,
                             const GLint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4iv\n";
  glProgramUniform4iv(program, location, count, value);
  gl_error_check("glProgramUniform4iv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_1uiv(GLuint program, GLint location, GLsizei count,
                              const GLuint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform1uiv\n";
  glProgramUniform1uiv(program, location, count, value);
  gl_error_check("glProgramUniform1uiv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_2uiv(GLuint program, GLint location, GLsizei count,
                              const GLuint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform2uiv\n";
  glProgramUniform2uiv(program, location, count, value);
  gl_error_check("glProgramUniform2uiv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_3uiv(GLuint program, GLint location, GLsizei count,
                              const GLuint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform3uiv\n";
  glProgramUniform3uiv(program, location, count, value);
  gl_error_check("glProgramUniform3uiv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_4uiv(GLuint program, GLint location, GLsizei count,
                              const GLuint* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniform4uiv\n";
  glProgramUniform4uiv(program, location, count, value);
  gl_error_check("glProgramUniform4uiv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_2fv(GLuint program, GLint location,
                                    GLsizei count, GLboolean transpose,
                                    const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix2fv\n";
  glProgramUniformMatrix2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_3fv(GLuint program, GLint location,
                                    GLsizei count, GLboolean transpose,
                                    const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix3fv\n";
  glProgramUniformMatrix3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_4fv(GLuint program, GLint location,
                                    GLsizei count, GLboolean transpose,
                                    const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix4fv\n";
  glProgramUniformMatrix4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_2x3fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix2x3fv\n";
  glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2x3fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_3x2fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix3x2fv\n";
  glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3x2fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_2x4fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix2x4fv\n";
  glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2x4fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_4x2fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix4x2fv\n";
  glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4x2fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_3x4fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix3x4fv\n";
  glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3x4fv");
}

//------------------------------------------------------------------------------

void gl::program_uniform_matrix_4x3fv(GLuint program, GLint location,
                                      GLsizei count, GLboolean transpose,
                                      const GLfloat* value) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glProgramUniformMatrix4x3fv\n";
  glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4x3fv");
}

//------------------------------------------------------------------------------

void gl::get_program_iv(GLuint program, GLenum pname, GLint* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetProgramiv\n";
  glGetProgramiv(program, pname, params);
  gl_error_check("glGetProgramiv");
}

//------------------------------------------------------------------------------

void gl::get_program_info_log(GLuint program, GLsizei maxLength,
                              GLsizei* length, GLchar* infoLog) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetProgramInfoLog\n";
  glGetProgramInfoLog(program, maxLength, length, infoLog);
  gl_error_check("glGetProgramInfoLog");
}

//------------------------------------------------------------------------------

GLint gl::get_uniform_location(GLuint program, const GLchar* name) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetUniformLocation\n";
  auto result = glGetUniformLocation(program, name);
  gl_error_check("glGetUniformLocation");
  return result;
}

//==============================================================================
// TEXTURE RELATED
//==============================================================================

void gl::create_textures(GLenum target, GLsizei n, GLuint* textures) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCreateTextures(target, n, textures);
  if (verbose) {
    *out << "glCreateTextures(" << to_string(target) << ", " << n << ", "
         << textures << ") = [ ";
    for (GLsizei i = 0; i < n; ++i) *out << textures[i] << ' ';
    *out << "]\n";
  }
  gl_error_check("glCreateTextures");
}

//------------------------------------------------------------------------------

void gl::delete_textures(GLsizei n, GLuint* textures) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateTextures\n";
  glDeleteTextures(n, textures);
  gl_error_check("glDeleteTextures");
}

//------------------------------------------------------------------------------

void gl::active_texture(GLenum texture) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glActiveTexture(" << texture - GL_TEXTURE0 << ")\n";
  glActiveTexture(texture);
  gl_error_check("glActiveTexture");
}

//------------------------------------------------------------------------------

void gl::bind_texture(GLenum target, GLuint texture) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glBindTexture(" << to_string(target) << ", " << texture << ")\n";
  glBindTexture(target, texture);
  gl_error_check("glBindTexture");
}

//------------------------------------------------------------------------------

void gl::bind_image_texture(GLuint unit, GLuint texture, GLint level,
                            GLboolean layered, GLint layer, GLenum access,
                            GLenum format) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glBindImageTexture(" << unit << ", " << texture << ", " << level
         << ", " << layered << ", " << layer << ", " << access << ", " << format
         << ")\n";
  glBindImageTexture(unit, texture, level, layered, layer, access, format);
  gl_error_check("glBindImageTexture");
}

//------------------------------------------------------------------------------

void gl::tex_image_2d(GLenum target, GLint level, GLint internalFormat,
                      GLsizei width, GLsizei height, GLint border,
                      GLenum format, GLenum type, const GLvoid* data) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose)
    *out << "glTexImage2D(" << to_string(target) << ", " << level << ", "
         << to_string(internalFormat) << ", " << width << ", " << height << ", " << border
         << ", " << to_string(format) << ", " << to_string(type) << ", " << data << ")\n";
  glTexImage2D(target, level, internalFormat, width, height, border, format,
               type, data);
  gl_error_check("glTexImage2D");
}

//------------------------------------------------------------------------------

void gl::tex_sub_image_2d(GLenum target, GLint level, GLint xoffset,
                          GLint yoffset, GLsizei width, GLsizei height,
                          GLenum format, GLenum type, const GLvoid* pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexSubImage2D\n";
  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type,
                  pixels);
  gl_error_check("glTexSubImage2D");
}

//------------------------------------------------------------------------------

void gl::texture_sub_image_2d(GLuint texture, GLint level, GLint xoffset,
                              GLint yoffset, GLsizei width, GLsizei height,
                              GLenum format, GLenum type, const void* pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexSubImage2D\n";
  glTexSubImage2D(texture, level, xoffset, yoffset, width, height, format, type,
                  pixels);
  gl_error_check("glTextureSubImage2D");
}

//------------------------------------------------------------------------------

void gl::get_tex_image(GLenum target, GLint level, GLenum format, GLenum type,
                       GLvoid* pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetTexImage\n";
  glGetTexImage(target, level, format, type, pixels);
  gl_error_check("glGetTexImage");
}

//------------------------------------------------------------------------------

void gl::get_n_tex_image(GLenum target, GLint level, GLenum format, GLenum type,
                         GLsizei bufSize, void* pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetnTexImage\n";
  glGetnTexImage(target, level, format, type, bufSize, pixels);
  gl_error_check("glGetnTexImage");
}

//------------------------------------------------------------------------------

void gl::get_texture_image(GLuint texture, GLint level, GLenum format,
                           GLenum type, GLsizei bufSize, void* pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glGetTextureImage\n";
  glGetTextureImage(texture, level, format, type, bufSize, pixels);
  gl_error_check("glGetTextureImage");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_f(GLenum target, GLenum pname, GLfloat param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameterf\n";
  glTexParameterf(target, pname, param);
  gl_error_check("glTexParameterf");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_i(GLenum target, GLenum pname, GLint param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameteri\n";
  glTexParameteri(target, pname, param);
  gl_error_check("glTexParameteri");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_f(GLuint texture, GLenum pname, GLfloat param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameterf\n";
  glTextureParameterf(texture, pname, param);
  gl_error_check("glTexParameterf");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_i(GLuint texture, GLenum pname, GLint param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameteri\n";
  glTextureParameteri(texture, pname, param);
  gl_error_check("glTexParameteri");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_fv(GLenum target, GLenum pname, const GLfloat* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameterfv\n";
  glTexParameterfv(target, pname, params);
  gl_error_check("glTexParameterfv");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_iv(GLenum target, GLenum pname, const GLint* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameteriv\n";
  glTexParameteriv(target, pname, params);
  gl_error_check("glTexParameteriv");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_Iiv(GLenum target, GLenum pname, const GLint* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameterIiv\n";
  glTexParameterIiv(target, pname, params);
  gl_error_check("glTexParameterIiv");
}

//------------------------------------------------------------------------------

void gl::tex_parameter_Iuiv(GLenum target, GLenum pname, const GLuint* params) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTexParameterIuiv\n";
  glTexParameterIuiv(target, pname, params);
  gl_error_check("glTexParameterIuiv");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_fv(GLuint texture, GLenum pname,
                              const GLfloat* paramtexture) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameterfv\n";
  glTextureParameterfv(texture, pname, paramtexture);
  gl_error_check("glTextureParameterfv");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_iv(GLuint texture, GLenum pname,
                              const GLint* param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameteriv\n";
  glTextureParameteriv(texture, pname, param);
  gl_error_check("glTextureParameteriv");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_Iiv(GLuint texture, GLenum pname,
                               const GLint* param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameterIiv\n";
  glTextureParameterIiv(texture, pname, param);
  gl_error_check("glTextureParameterIiv");
}

//------------------------------------------------------------------------------

void gl::texture_parameter_Iuiv(GLuint texture, GLenum pname,
                                const GLuint* param) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glTextureParameterIuiv\n";
  glTextureParameterIuiv(texture, pname, param);
  gl_error_check("glTextureParameterIuiv");
}

//==============================================================================
// FRAMEBUFFER RELATED
//==============================================================================

void gl::create_framebuffers(GLsizei n, GLuint* ids) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCreateFramebuffers\n";
  glCreateFramebuffers(n, ids);
  gl_error_check("glCreateFramebuffers");
}

//------------------------------------------------------------------------------

void gl::delete_framebuffers(GLsizei n, GLuint* ids) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glDeleteFramebuffers\n";
  glDeleteFramebuffers(n, ids);
  gl_error_check("glDeleteFramebuffers");
}

//------------------------------------------------------------------------------

void gl::bind_framebuffer(GLenum target, GLuint framebuffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glBindFramebuffer\n";
  glBindFramebuffer(target, framebuffer);
  gl_error_check("glBindFramebuffer");
}

//------------------------------------------------------------------------------

void gl::framebuffer_texture(GLenum target, GLenum attachment, GLuint texture,
                             GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glFramebufferTexture\n";
  glFramebufferTexture(target, attachment, texture, level);
  gl_error_check("glFramebufferTexture");
}

//------------------------------------------------------------------------------

void gl::framebuffer_texture_1d(GLenum target, GLenum attachment,
                                GLenum textarget, GLuint texture, GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glFramebufferTexture1D\n";
  glFramebufferTexture1D(target, attachment, textarget, texture, level);
  gl_error_check("glFramebufferTexture1D");
}

//------------------------------------------------------------------------------

void gl::framebuffer_texture_2d(GLenum target, GLenum attachment,
                                GLenum textarget, GLuint texture, GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glFramebufferTexture2D\n";
  glFramebufferTexture2D(target, attachment, textarget, texture, level);
  gl_error_check("glFramebufferTexture2D");
}

//------------------------------------------------------------------------------

void gl::framebuffer_texture_3d(GLenum target, GLenum attachment,
                                GLenum textarget, GLuint texture, GLint level,
                                GLint layer) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glFramebufferTexture3D\n";
  glFramebufferTexture3D(target, attachment, textarget, texture, level, layer);
  gl_error_check("glFramebufferTexture3D");
}

//------------------------------------------------------------------------------

void gl::named_framebuffer_texture(GLuint framebuffer, GLenum attachment,
                                   GLuint texture, GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glNamedFramebufferTexture\n";
  glNamedFramebufferTexture(framebuffer, attachment, texture, level);
  gl_error_check("glNamedFramebufferTexture");
}

//------------------------------------------------------------------------------

GLenum gl::check_named_framebuffer_status(GLuint framebuffer, GLenum target) {
  std::lock_guard lock(detail::mutex::gl_call);
  if (verbose) *out << "glCheckNamedFramebufferStatus\n";
  auto result = glCheckNamedFramebufferStatus(framebuffer, target);
  gl_error_check("glCheckNamedFramebufferStatus");
  return result;
}
//==============================================================================
}  // namespace Yavin
//==============================================================================
