#ifndef __YAVIN_GEOMETRYSHADER_H__
#define __YAVIN_GEOMETRYSHADER_H__

#include <string>

#include "ShaderStage.h"
#include "dll_export.h"

namespace Yavin {
class GeometryShader : public ShaderStage {
 public:
  DLL_API GeometryShader(const std::string& filename);
  DLL_API GeometryShader(GeometryShader&& other);
};
}  // namespace Yavin

#endif