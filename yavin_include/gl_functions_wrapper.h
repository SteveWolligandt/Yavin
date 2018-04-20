#ifndef __YAVIN_GL_FUNCTIONS_WRAPPER_H__
#define __YAVIN_GL_FUNCTIONS_WRAPPER_H__

#include "error_check.h"
#include "mutex_handler.h"

//==============================================================================
namespace Yavin::gl {
//==============================================================================

//==============================================================================
// BUFFER RELATED
//==============================================================================

inline void bind_buffer(GLenum target, GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindBuffer(target, buffer);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

inline void create_buffers(GLsizei n, GLuint *buffers) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCreateBuffers(1, buffers);
  gl_error_check("glCreateBuffers");
}

//------------------------------------------------------------------------------

inline void delete_buffers(GLsizei n, GLuint *buffers) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDeleteBuffers(1, buffers);
  gl_error_check("glCreateBuffers");
}

//------------------------------------------------------------------------------

inline void copy_named_buffer_sub_data(GLuint readBuffer, GLuint writeBuffer,
                                       GLintptr readOffset,
                                       GLintptr writeOffset, GLsizei size) {
  std::lock_guard lock(detail::mutex::buffer);
  glCopyNamedBufferSubData(readBuffer, writeBuffer, readOffset, writeOffset,
                           size);
  gl_error_check("glCopyNamedBufferSubData");
}

//------------------------------------------------------------------------------

inline auto map_named_buffer_range(GLuint buffer, GLintptr offset,
                                   GLsizei length, GLbitfield access) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto result = glMapNamedBufferRange(buffer, offset, length, access);
  gl_error_check("glMapNamedBufferRange");
  return result;
}

//------------------------------------------------------------------------------

inline void named_buffer_data(GLuint buffer, GLsizei size, const void *data,
                              GLenum usage) {
  std::lock_guard lock(detail::mutex::gl_call);
  glNamedBufferData(buffer, size, data, usage);
  gl_error_check("glNamedBufferData");
}

//------------------------------------------------------------------------------

inline auto unmap_named_buffer(GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glUnmapNamedBuffer(buffer);
  gl_error_check("glUnmapNamedBuffer");
  return result;
}

//------------------------------------------------------------------------------

inline void named_buffer_sub_data(GLuint buffer, GLintptr offset, GLsizei size,
                                  const void *data) {
  std::lock_guard lock(detail::mutex::gl_call);
  glNamedBufferSubData(buffer, offset, size, data);
  gl_error_check("glNamedBufferSubData");
}

//==============================================================================
// SHADER RELATED
//==============================================================================

inline auto create_program() {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glCreateProgram();
  gl_error_check("glCreateProgram");
  return result;
}

//------------------------------------------------------------------------------

inline auto attach_shader(GLuint program, GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  glAttachShader(program, shader);
  gl_error_check("glAttachShader");
}

//------------------------------------------------------------------------------

inline auto link_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  glLinkProgram(program);
  gl_error_check("glLinkProgram");
}

//------------------------------------------------------------------------------

inline auto delete_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDeleteProgram(program);
  gl_error_check("glDeleteProgram");
}

//------------------------------------------------------------------------------

inline auto use_program(GLuint program) {
  std::lock_guard lock(detail::mutex::gl_call);
  glUseProgram(program);
  gl_error_check("glUseProgram");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1f(GLuint program, GLint location, GLfloat v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1f(program, location, v0);
  gl_error_check("glProgramUniform1f");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2f(GLuint program, GLint location, GLfloat v0,
                               GLfloat v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2f(program, location, v0, v1);
  gl_error_check("glProgramUniform2f");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3f(GLuint program, GLint location, GLfloat v0,
                               GLfloat v1, GLfloat v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3f(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3f");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4f(GLuint program, GLint location, GLfloat v0,
                               GLfloat v1, GLfloat v2, GLfloat v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4f(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4f");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1i(GLuint program, GLint location, GLint v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1i(program, location, v0);
  gl_error_check("glProgramUniform1i");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2i(GLuint program, GLint location, GLint v0,
                               GLint v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2i(program, location, v0, v1);
  gl_error_check("glProgramUniform2i");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3i(GLuint program, GLint location, GLint v0,
                               GLint v1, GLint v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3i(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3i");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4i(GLuint program, GLint location, GLint v0,
                               GLint v1, GLint v2, GLint v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4i(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4i");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1ui(GLuint program, GLint location, GLuint v0) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1ui(program, location, v0);
  gl_error_check("glProgramUniform1ui");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2ui(GLuint program, GLint location, GLuint v0,
                                GLuint v1) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2ui(program, location, v0, v1);
  gl_error_check("glProgramUniform2ui");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3ui(GLuint program, GLint location, GLuint v0,
                                GLuint v1, GLuint v2) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3ui(program, location, v0, v1, v2);
  gl_error_check("glProgramUniform3ui");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4ui(GLuint program, GLint location, GLuint v0,
                                GLuint v1, GLuint v2, GLuint v3) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4ui(program, location, v0, v1, v2, v3);
  gl_error_check("glProgramUniform4ui");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1fv(GLuint program, GLint location, GLsizei count,
                                const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1fv(program, location, count, value);
  gl_error_check("glProgramUniform1fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2fv(GLuint program, GLint location, GLsizei count,
                                const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2fv(program, location, count, value);
  gl_error_check("glProgramUniform2fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3fv(GLuint program, GLint location, GLsizei count,
                                const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3fv(program, location, count, value);
  gl_error_check("glProgramUniform3fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4fv(GLuint program, GLint location, GLsizei count,
                                const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4fv(program, location, count, value);
  gl_error_check("glProgramUniform4fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1iv(GLuint program, GLint location, GLsizei count,
                                const GLint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1iv(program, location, count, value);
  gl_error_check("glProgramUniform1iv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2iv(GLuint program, GLint location, GLsizei count,
                                const GLint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2iv(program, location, count, value);
  gl_error_check("glProgramUniform2iv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3iv(GLuint program, GLint location, GLsizei count,
                                const GLint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3iv(program, location, count, value);
  gl_error_check("glProgramUniform3iv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4iv(GLuint program, GLint location, GLsizei count,
                                const GLint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4iv(program, location, count, value);
  gl_error_check("glProgramUniform4iv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_1uiv(GLuint program, GLint location, GLsizei count,
                                 const GLuint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform1uiv(program, location, count, value);
  gl_error_check("glProgramUniform1uiv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_2uiv(GLuint program, GLint location, GLsizei count,
                                 const GLuint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform2uiv(program, location, count, value);
  gl_error_check("glProgramUniform2uiv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_3uiv(GLuint program, GLint location, GLsizei count,
                                 const GLuint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform3uiv(program, location, count, value);
  gl_error_check("glProgramUniform3uiv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_4uiv(GLuint program, GLint location, GLsizei count,
                                 const GLuint *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniform4uiv(program, location, count, value);
  gl_error_check("glProgramUniform4uiv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_2fv(GLuint program, GLint location,
                                       GLsizei count, GLboolean transpose,
                                       const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_3fv(GLuint program, GLint location,
                                       GLsizei count, GLboolean transpose,
                                       const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_4fv(GLuint program, GLint location,
                                       GLsizei count, GLboolean transpose,
                                       const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_2x3fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix2x3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2x3fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_3x2fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix3x2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3x2fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_2x4fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix2x4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix2x4fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_4x2fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix4x2fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4x2fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_3x4fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix3x4fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix3x4fv");
}

//------------------------------------------------------------------------------

inline auto program_uniform_matrix_4x3fv(GLuint program, GLint location,
                                         GLsizei count, GLboolean transpose,
                                         const GLfloat *value) {
  std::lock_guard lock(detail::mutex::gl_call);
  glProgramUniformMatrix4x3fv(program, location, count, transpose, value);
  gl_error_check("glProgramUniformMatrix4x3fv");
}

//------------------------------------------------------------------------------

inline auto get_program_iv(GLuint program, GLenum pname, GLint *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetProgramiv(program, pname, params);
  gl_error_check("glGetProgramiv");
}

//------------------------------------------------------------------------------

inline auto get_program_info_log(GLuint program, GLsizei maxLength,
                                 GLsizei *length, GLchar *infoLog) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetProgramInfoLog(program, maxLength, length, infoLog);
  gl_error_check("glGetProgramInfoLog");
}

//==============================================================================
}  // namespace Yavin::gl
//==============================================================================

#endif
