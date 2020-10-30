#ifndef YAVIN_GEOMETRYSHADER_H
#define YAVIN_GEOMETRYSHADER_H
//==============================================================================
#include <string>

#include "dllexport.h"
#include "shaderstage.h"
//==============================================================================
namespace yavin {
//==============================================================================
class geometryshader : public shaderstage {
 public:
  DLL_API geometryshader(std::string const& filename,
                         shadersourcetype   string_type = FILE);
  DLL_API geometryshader(std::string_view const& filename,
                         shadersourcetype        string_type = FILE);
  DLL_API geometryshader(char const*      filename,
                         shadersourcetype string_type = FILE);
  DLL_API geometryshader(geometryshader&& other);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
