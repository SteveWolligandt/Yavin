#ifndef __YAVIN_ERROR_CHECK_H__
#define __YAVIN_ERROR_CHECK_H__

#include <stdexcept>
#include <string>
#include "compilation.h"

namespace Yavin {
class gl_error : public std::runtime_error {
 public:
  gl_error(const std::string& function_name, const std::string& message);
};

class gl_framebuffer_not_complete_error : public std::runtime_error {
 public:
  gl_framebuffer_not_complete_error(const std::string& what);
};

const std::string gl_error_to_string(int err);
const std::string gl_framebuffer_error_to_string(int status);

void gl_error_check(const std::string& function);
void gl_framebuffer_not_complete_check(const unsigned int fbo_id);
}  // Yavin
#endif