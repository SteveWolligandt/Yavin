#ifndef __YAVIN_GEOMETRYSHADER_H__
#define __YAVIN_GEOMETRYSHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

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
