#ifndef YAVIN_TESSELATIONCONTROLSHADER_H
#define YAVIN_TESSELATIONCONTROLSHADER_H
//==============================================================================
#include <string>

#include "dllexport.h"
#include "shaderstage.h"
//==============================================================================
namespace yavin {
//==============================================================================
class tesselationcontrolshader : public shaderstage {
 public:
  DLL_API tesselationcontrolshader(
      std::string const& filename,
      shadersourcetype   string_type = shadersourcetype::FILE);
  DLL_API tesselationcontrolshader(
      std::string_view const& filename,
      shadersourcetype        string_type = shadersourcetype::FILE);
  DLL_API tesselationcontrolshader(
      char const*      filename,
      shadersourcetype string_type = shadersourcetype::FILE);
  DLL_API tesselationcontrolshader(tesselationcontrolshader&& other);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
