#ifndef YAVIN_TESSELATIONEVALUATIONSHADER_H
#define YAVIN_TESSELATIONEVALUATIONSHADER_H
//==============================================================================
#include <string>

#include "dllexport.h"
#include "shaderstage.h"
//==============================================================================
namespace yavin {
//==============================================================================
class tesselationevaluationshader : public shaderstage {
 public:
  DLL_API tesselationevaluationshader(
      std::string const& filename,
      shadersourcetype   string_type = shadersourcetype::FILE);
  DLL_API tesselationevaluationshader(
      std::string_view const& filename,
      shadersourcetype        string_type = shadersourcetype::FILE);
  DLL_API tesselationevaluationshader(
      char const*      filename,
      shadersourcetype string_type = shadersourcetype::FILE);
  DLL_API tesselationevaluationshader(tesselationevaluationshader&& other);
};
//==============================================================================
}  // namespace yavin
//==============================================================================
#endif
