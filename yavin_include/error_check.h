#ifndef __YAVIN_ERROR_CHECK_H__
#define __YAVIN_ERROR_CHECK_H__

#include <stdexcept>
#include <string>
#include "compilation.h"
#include "dll_export.h"

namespace Yavin {
class gl_error : public std::runtime_error {
 public:
  DLL_API gl_error(const std::string& function_name, const std::string& message);
};

class gl_framebuffer_not_complete_error : public std::runtime_error {
 public:
	 DLL_API gl_framebuffer_not_complete_error(const std::string& what);
};

DLL_API const std::string gl_error_to_string(int err);
DLL_API const std::string gl_framebuffer_error_to_string(int status);

DLL_API void gl_error_check(const std::string& function);
DLL_API void gl_framebuffer_not_complete_check(const unsigned int fbo_id);

}  // namespace Yavin
#endif