#ifndef __YAVIN_VERTEXSHADER_H__
#define __YAVIN_VERTEXSHADER_H__

#include <string>
#include "gl_includes.h"

#include "ShaderStage.h"
#include "dll_export.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class VertexShader : public ShaderStage {
 public:
  DLL_API VertexShader(const std::string& filename);
  DLL_API VertexShader(VertexShader&& other);
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
