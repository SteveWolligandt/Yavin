#include <yavin/ansi_format.h>
#include <yavin/error_check.h>
#include <iostream>
#include <sstream>

#include <yavin/gl_functions.h>

//==============================================================================
namespace yavin {
//==============================================================================

gl_error::gl_error(const std::string& function_name, const std::string& message,
                   const std::string& file, size_t line)
    : std::runtime_error(ansi::red + ansi::bold + "[" + function_name + "] " +
                         ansi::reset + ansi::cyan + file + ansi::reset + ":" +
                         ansi::yellow + std::to_string(line) + ansi::reset +
                         "\n  " + message) {}

//------------------------------------------------------------------------------

gl_framebuffer_not_complete_error::gl_framebuffer_not_complete_error(
    const std::string& what)
    : std::runtime_error("[FrameBuffer incomplete] " + what) {}

//------------------------------------------------------------------------------

const std::string gl_error_to_string(GLenum err) {
  switch (err) {
    case GL_INVALID_ENUM: return "invalid enum";
    case GL_INVALID_VALUE: return "invalid value";
    case GL_INVALID_OPERATION: return "invalid operation";
    case GL_STACK_OVERFLOW: return "stack overflow";
    case GL_STACK_UNDERFLOW: return "stack underflow";
    case GL_OUT_OF_MEMORY:
      return "out of memory";
      // case GL_TABLE_TOO_LARGE:
      //   return "table too large";
  }
  return "";
}

//------------------------------------------------------------------------------

const std::string gl_framebuffer_error_to_string(GLenum status) {
  switch (status) {
    case GL_FRAMEBUFFER_UNDEFINED: return "undefined";
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT: return "incomplete attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
      return "incomplete missing attachment";
    case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER: return "incomplete draw buffer";
    case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER: return "incomplete read buffer";
    case GL_FRAMEBUFFER_UNSUPPORTED: return "unsupported";
    case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE: return "incomplete multisample";
    case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
      return "incomplete layer targets";
  }
  return "";
}

//------------------------------------------------------------------------------

void _gl_error_check(const std::string& function, const char* file,
                     size_t line) {
  auto err     = gl::get_error();
  auto err_str = gl_error_to_string(err);
  if (err != GL_NO_ERROR)
    throw gl_error(function, err_str, file, line);
}

//------------------------------------------------------------------------------

void gl_framebuffer_not_complete_check(const GLuint fbo_id) {
  auto status     = gl::check_named_framebuffer_status(fbo_id, GL_FRAMEBUFFER);
  auto status_str = gl_framebuffer_error_to_string(status);
  if (status != GL_FRAMEBUFFER_COMPLETE)
    throw gl_framebuffer_not_complete_error(status_str);
}

//==============================================================================
}  // namespace yavin
//==============================================================================