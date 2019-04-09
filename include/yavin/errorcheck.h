#ifndef __YAVIN_ERROR_CHECK_H__
#define __YAVIN_ERROR_CHECK_H__

#include <stdexcept>
#include <string>
#include "compilation.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class gl_error : public std::runtime_error {
 public:
  DLL_API gl_error(const std::string& function_name, const std::string& message,
                   const std::string& file, size_t line);
};

class gl_framebuffer_not_complete_error : public std::runtime_error {
 public:
  DLL_API explicit gl_framebuffer_not_complete_error(const std::string& what);
};

DLL_API const std::string gl_error_to_string(int err);
DLL_API const std::string gl_framebuffer_error_to_string(int status);

DLL_API void _gl_error_check(const std::string& function, const char* file,
                             size_t line);
DLL_API void gl_framebuffer_not_complete_check(const unsigned int fbo_id);

#define gl_error_check(fun) _gl_error_check(fun, __FILE__, __LINE__);

//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
