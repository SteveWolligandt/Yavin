#ifndef __YAVIN_GEOMETRYSHADER_H__
#define __YAVIN_GEOMETRYSHADER_H__

#include <string>

#include "shader_stage.h"
#include "dll_export.h"

//==============================================================================
namespace yavin {
//==============================================================================

class GeometryShader : public ShaderStage {
 public:
  DLL_API GeometryShader(const std::string& filename);
  DLL_API GeometryShader(GeometryShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
