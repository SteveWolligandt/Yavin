#ifndef __YAVIN_GL_FUNCTIONS_WRAPPER_H__
#define __YAVIN_GL_FUNCTIONS_WRAPPER_H__

#include <vector>
#include "Camera.h"
#include "error_check.h"
#include "gl_includes.h"
#include "mutex_handler.h"

//==============================================================================
namespace Yavin::gl {
//==============================================================================

//==============================================================================
// MISC
//==============================================================================

inline void enable(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  glEnable(cap);
  gl_error_check("glEnable");
}

//------------------------------------------------------------------------------

inline auto is_enabled(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glIsEnabled(cap);
  gl_error_check("glIsEnabled");
  return result;
}

//------------------------------------------------------------------------------

inline void disable(GLenum cap) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDisable(cap);
  gl_error_check("glDisable");
}

//------------------------------------------------------------------------------

inline auto get_string(GLenum name) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glGetString(name);
  gl_error_check("glGetString");
  return (char *)result;
}

//==============================================================================
// BACKBUFFER RELATED
//==============================================================================

inline void clear(GLbitfield mask) {
  std::lock_guard lock(detail::mutex::gl_call);
  glClear(mask);
  gl_error_check("glClear");
}

//------------------------------------------------------------------------------

inline void clear_color(GLfloat red, GLfloat green, GLfloat blue,
                        GLfloat alpha) {
  std::lock_guard lock(detail::mutex::gl_call);
  glClearColor(red, green, blue, alpha);
  gl_error_check("glClearColor");
}

//------------------------------------------------------------------------------

inline void viewport(GLint x, GLint y, GLsizei width, GLsizei height) {
  std::lock_guard lock(detail::mutex::gl_call);
  glViewport(x, y, width, height);
  gl_error_check("glViewport");
}

//------------------------------------------------------------------------------

inline void viewport(const glm::vec4 &vp) {
  viewport(vp[0], vp[1], vp[2], vp[3]);
}

//------------------------------------------------------------------------------

inline void flush() {
  std::lock_guard lock(detail::mutex::gl_call);
  glFlush();
  gl_error_check("glFlush");
}

//------------------------------------------------------------------------------

inline void depth_mask(GLboolean flag) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDepthMask(flag);
  gl_error_check("glDepthMask");
}

//------------------------------------------------------------------------------

inline void blend_func(GLenum sfactor, GLenum dfactor) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBlendFunc(sfactor, dfactor);
  gl_error_check("glBlendFunc");
}

//------------------------------------------------------------------------------

inline void blend_funci(GLuint buf, GLenum sfactor, GLenum dfactor) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBlendFunci(buf, sfactor, dfactor);
  gl_error_check("glBlendFunci");
}

//==============================================================================
// VERTEXBUFFER RELATED
//==============================================================================

inline void enable_vertex_attrib_array(GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  glEnableVertexAttribArray(index);
  gl_error_check("glEnableVertexAttribArray");
}

//------------------------------------------------------------------------------

inline void disable_vertex_attrib_array(GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDisableVertexAttribArray(index);
  gl_error_check("glDisableVertexAttribArray");
}

//------------------------------------------------------------------------------

inline void enable_vertex_attrib_array(GLuint vaobj, GLuint index) {
  std::lock_guard lock(detail::mutex::gl_call);
  glEnableVertexArrayAttrib(vaobj, index);
  gl_error_check("glEnableVertexArrayAttrib");
}

//------------------------------------------------------------------------------

// inline void disable_vertex_attrib_array(GLuint vaobj, GLuint index) {
//  std::lock_guard lock(detail::mutex::gl_call);
//  glDisableVertexArrayAttrib(vaobj, index);
//  gl_error_check("glDisableVertexArrayAttrib");
//}

//------------------------------------------------------------------------------

inline void vertex_attrib_pointer(GLuint index, GLint size, GLenum type,
                                  GLboolean normalized, GLsizei stride,
                                  const GLvoid *pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  gl_error_check("glVertexAttribPointer");
}

//------------------------------------------------------------------------------

inline void vertex_attrib_i_pointer(GLuint index, GLint size, GLenum type,
                                    GLsizei stride, const GLvoid *pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glVertexAttribIPointer(index, size, type, stride, pointer);
  gl_error_check("glVertexAttribIPointer");
}

//------------------------------------------------------------------------------

inline void vertex_attrib_l_pointer(GLuint index, GLint size, GLenum type,
                                    GLsizei stride, const GLvoid *pointer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glVertexAttribLPointer(index, size, type, stride, pointer);
  gl_error_check("glVertexAttribLPointer");
}

//==============================================================================
// VERTEXARRAY RELATED
//==============================================================================

inline void create_vertex_arrays(GLsizei n, GLuint *arrays) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCreateVertexArrays(n, arrays);
  gl_error_check("glCreateVertexArrays");
}

//------------------------------------------------------------------------------

inline void delete_vertex_arrays(GLsizei n, GLuint *arrays) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDeleteVertexArrays(n, arrays);
  gl_error_check("glDeleteVertexArrays");
}

//------------------------------------------------------------------------------

inline void bind_vertex_array(GLuint array) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindVertexArray(array);
  gl_error_check("glBindVertexArray");
}

//------------------------------------------------------------------------------

inline void draw_elements(GLenum mode, GLsizei count, GLenum type,
                          const GLvoid *indices) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDrawElements(mode, count, type, indices);
  gl_error_check("glDrawElements");
}

//==============================================================================
// BUFFER RELATED
//==============================================================================

inline void bind_buffer(GLenum target, GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindBuffer(target, buffer);
  gl_error_check("glBindBuffer");
}

//------------------------------------------------------------------------------

inline void bind_buffer_base(GLenum target, GLuint index, GLuint buffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindBufferBase(target, index, buffer);
  gl_error_check("glBindBufferBase");
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

inline auto map_named_buffer(GLuint buffer, GLenum access) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glMapNamedBuffer(buffer, access);
  gl_error_check("glMapNamedBuffer");
  return result;
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

inline auto create_shader(GLenum shaderType) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glCreateShader(shaderType);
  gl_error_check("glCreateShader");
  return result;
}

//------------------------------------------------------------------------------

inline auto shader_source(GLuint shader, GLsizei count, const GLchar **string,
                          const GLint *length) {
  std::lock_guard lock(detail::mutex::gl_call);
  glShaderSource(shader, count, string, length);
  gl_error_check("glShaderSource");
}

//------------------------------------------------------------------------------

inline auto compile_shader(GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCompileShader(shader);
  gl_error_check("glCompileShader");
}

//------------------------------------------------------------------------------

inline auto delete_shader(GLuint shader) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDeleteShader(shader);
  gl_error_check("glDeleteShader");
}

//------------------------------------------------------------------------------

inline auto get_shader_iv(GLuint shader, GLenum pname, GLint *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetShaderiv(shader, pname, params);
  gl_error_check("glGetShaderiv");
}

//------------------------------------------------------------------------------

inline auto get_shader_info_log_length(GLuint shader) {
  GLint info_log_length;
  get_shader_iv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
  return info_log_length;
}

//------------------------------------------------------------------------------

inline auto get_shader_info_log(GLuint shader, GLsizei maxLength,
                                GLsizei *length, GLchar *infoLog) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetShaderInfoLog(shader, maxLength, length, infoLog);
  gl_error_check("glGetShaderInfoLog");
}

//------------------------------------------------------------------------------

inline auto get_shader_info_log(GLuint shader, GLsizei maxLength) {
  std::vector<char> buffer(maxLength);
  get_shader_info_log(shader, maxLength, nullptr, buffer.data());
  std::string log(buffer.begin(), buffer.end());
  return log;
}

//------------------------------------------------------------------------------

inline auto get_shader_info_log(GLuint shader) {
  return get_shader_info_log(shader, get_shader_info_log_length(shader));
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

//------------------------------------------------------------------------------

inline auto get_uniform_location(GLuint program, const GLchar *name) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glGetUniformLocation(program, name);
  gl_error_check("glGetUniformLocation");
  return result;
}

//------------------------------------------------------------------------------

inline auto get_error() {
  auto result = glGetError();
  return result;
}

//==============================================================================
// TEXTURE RELATED
//==============================================================================

inline auto create_textures(GLenum target, GLsizei n, GLuint *textures) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCreateTextures(target, n, textures);
  gl_error_check("glCreateTextures");
}

//------------------------------------------------------------------------------

inline auto active_texture(GLenum texture) {
  std::lock_guard lock(detail::mutex::gl_call);
  glActiveTexture(texture);
  gl_error_check("glActiveTexture");
}

//------------------------------------------------------------------------------

inline auto bind_texture(GLenum target, GLuint texture) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindTexture(target, texture);
  gl_error_check("glBindTexture");
}

//------------------------------------------------------------------------------

inline auto bind_image_texture(GLuint unit, GLuint texture, GLint level,
                               GLboolean layered, GLint layer, GLenum access,
                               GLenum format) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindImageTexture(unit, texture, level, layered, layer, access, format);
  gl_error_check("glBindImageTexture");
}

//------------------------------------------------------------------------------

inline auto tex_image_2d(GLenum target, GLint level, GLint internalFormat,
                         GLsizei width, GLsizei height, GLint border,
                         GLenum format, GLenum type, const GLvoid *data) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexImage2D(target, level, internalFormat, width, height, border, format,
               type, data);
  gl_error_check("glTexImage2D");
}

//------------------------------------------------------------------------------

inline auto tex_sub_image_2d(GLenum target, GLint level, GLint xoffset,
                             GLint yoffset, GLsizei width, GLsizei height,
                             GLenum format, GLenum type, const GLvoid *pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type,
                  pixels);
  gl_error_check("glTexSubImage2D");
}

//------------------------------------------------------------------------------

inline auto texture_sub_image_2d(GLuint texture, GLint level, GLint xoffset,
                                 GLint yoffset, GLsizei width, GLsizei height,
                                 GLenum format, GLenum type,
                                 const void *pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexSubImage2D(texture, level, xoffset, yoffset, width, height, format, type,
                  pixels);
  gl_error_check("glTextureSubImage2D");
}

//------------------------------------------------------------------------------

inline auto get_tex_image(GLenum target, GLint level, GLenum format,
                          GLenum type, GLvoid *pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetTexImage(target, level, format, type, pixels);
  gl_error_check("glGetTexImage");
}

//------------------------------------------------------------------------------

inline auto get_n_tex_image(GLenum target, GLint level, GLenum format,
                            GLenum type, GLsizei bufSize, void *pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetnTexImage(target, level, format, type, bufSize, pixels);
  gl_error_check("glGetnTexImage");
}

//------------------------------------------------------------------------------

inline auto get_texture_image(GLuint texture, GLint level, GLenum format,
                              GLenum type, GLsizei bufSize, void *pixels) {
  std::lock_guard lock(detail::mutex::gl_call);
  glGetTextureImage(texture, level, format, type, bufSize, pixels);
  gl_error_check("glGetTextureImage");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_f(GLenum target, GLenum pname, GLfloat param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameterf(target, pname, param);
  gl_error_check("glTexParameterf");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_i(GLenum target, GLenum pname, GLint param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameteri(target, pname, param);
  gl_error_check("glTexParameteri");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_f(GLuint texture, GLenum pname, GLfloat param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameterf(texture, pname, param);
  gl_error_check("glTexParameterf");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_i(GLuint texture, GLenum pname, GLint param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameteri(texture, pname, param);
  gl_error_check("glTexParameteri");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_fv(GLenum target, GLenum pname,
                             const GLfloat *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameterfv(target, pname, params);
  gl_error_check("glTexParameterfv");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_iv(GLenum target, GLenum pname, const GLint *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameteriv(target, pname, params);
  gl_error_check("glTexParameteriv");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_Iiv(GLenum target, GLenum pname,
                              const GLint *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameterIiv(target, pname, params);
  gl_error_check("glTexParameterIiv");
}

//------------------------------------------------------------------------------

inline auto tex_parameter_Iuiv(GLenum target, GLenum pname,
                               const GLuint *params) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTexParameterIuiv(target, pname, params);
  gl_error_check("glTexParameterIuiv");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_fv(GLuint texture, GLenum pname,
                                 const GLfloat *paramtexture) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameterfv(texture, pname, paramtexture);
  gl_error_check("glTextureParameterfv");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_iv(GLuint texture, GLenum pname,
                                 const GLint *param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameteriv(texture, pname, param);
  gl_error_check("glTextureParameteriv");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_Iiv(GLuint texture, GLenum pname,
                                  const GLint *param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameterIiv(texture, pname, param);
  gl_error_check("glTextureParameterIiv");
}

//------------------------------------------------------------------------------

inline auto texture_parameter_Iuiv(GLuint texture, GLenum pname,
                                   const GLuint *param) {
  std::lock_guard lock(detail::mutex::gl_call);
  glTextureParameterIuiv(texture, pname, param);
  gl_error_check("glTextureParameterIuiv");
}

//==============================================================================
// FRAMEBUFFER RELATED
//==============================================================================

inline auto create_framebuffers(GLsizei n, GLuint *ids) {
  std::lock_guard lock(detail::mutex::gl_call);
  glCreateFramebuffers(n, ids);
  gl_error_check("glCreateFramebuffers");
}

//------------------------------------------------------------------------------

inline auto delete_framebuffers(GLsizei n, GLuint *ids) {
  std::lock_guard lock(detail::mutex::gl_call);
  glDeleteFramebuffers(n, ids);
  gl_error_check("glDeleteFramebuffers");
}

//------------------------------------------------------------------------------

inline auto bind_framebuffer(GLenum target, GLuint framebuffer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glBindFramebuffer(target, framebuffer);
  gl_error_check("glBindFramebuffer");
}

//------------------------------------------------------------------------------

inline auto framebuffer_texture(GLenum target, GLenum attachment,
                                GLuint texture, GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  glFramebufferTexture(target, attachment, texture, level);
  gl_error_check("glFramebufferTexture");
}

//------------------------------------------------------------------------------

inline auto framebuffer_texture_1d(GLenum target, GLenum attachment,
                                   GLenum textarget, GLuint texture,
                                   GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  glFramebufferTexture1D(target, attachment, textarget, texture, level);
  gl_error_check("glFramebufferTexture1D");
}

//------------------------------------------------------------------------------

inline auto framebuffer_texture_2d(GLenum target, GLenum attachment,
                                   GLenum textarget, GLuint texture,
                                   GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  glFramebufferTexture2D(target, attachment, textarget, texture, level);
  gl_error_check("glFramebufferTexture2D");
}

//------------------------------------------------------------------------------

inline auto framebuffer_texture_3d(GLenum target, GLenum attachment,
                                   GLenum textarget, GLuint texture,
                                   GLint level, GLint layer) {
  std::lock_guard lock(detail::mutex::gl_call);
  glFramebufferTexture3D(target, attachment, textarget, texture, level, layer);
  gl_error_check("glFramebufferTexture3D");
}

//------------------------------------------------------------------------------

inline auto named_framebuffer_texture(GLuint framebuffer, GLenum attachment,
                                      GLuint texture, GLint level) {
  std::lock_guard lock(detail::mutex::gl_call);
  glNamedFramebufferTexture(framebuffer, attachment, texture, level);
  gl_error_check("glNamedFramebufferTexture");
}

//------------------------------------------------------------------------------

inline auto check_named_framebuffer_status(GLuint framebuffer, GLenum target) {
  std::lock_guard lock(detail::mutex::gl_call);
  auto            result = glCheckNamedFramebufferStatus(framebuffer, target);
  gl_error_check("glCheckNamedFramebufferStatus");
  return result;
}

//==============================================================================
}  // namespace Yavin::gl
//==============================================================================

#endif
