#ifndef __YAVIN_VERTEXSHADER_H__
#define __YAVIN_VERTEXSHADER_H__

#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class VertexShader : public ShaderStage {
 public:
  DLL_API VertexShader(const std::string& filename);
  DLL_API VertexShader(VertexShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
