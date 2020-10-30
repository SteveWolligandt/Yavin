#ifndef YAVIN_COMPUTESHADER_H
#define YAVIN_COMPUTESHADER_H
//==============================================================================
#include <string>

#include "shaderstage.h"
#include "dllexport.h"
//==============================================================================
namespace yavin {
//==============================================================================
class computeshader : public shaderstage {
 public:
  DLL_API computeshader(std::string const& filename,
                        shadersourcetype string_type = shadersourcetype::FILE);
  DLL_API computeshader(std::string_view const& filename,
                        shadersourcetype string_type = shadersourcetype::FILE);
  DLL_API computeshader(char const*      filename,
                        shadersourcetype string_type = shadersourcetype::FILE);
  DLL_API computeshader(computeshader&& other);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
