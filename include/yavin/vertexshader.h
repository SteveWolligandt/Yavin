#ifndef YAVIN_VERTEXSHADER_H
#define YAVIN_VERTEXSHADER_H
//==============================================================================
#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"
//==============================================================================
namespace yavin {
//==============================================================================
class vertexshader : public shaderstage {
 public:
  DLL_API vertexshader(std::string const& filename,
                       shadersourcetype   string_type = FILE);
  DLL_API vertexshader(std::string_view const& filename,
                       shadersourcetype        string_type = FILE);
  DLL_API vertexshader(char const*      filename,
                       shadersourcetype string_type = FILE);
  DLL_API vertexshader(vertexshader&& other);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
