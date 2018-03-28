#ifndef __YAVIN_GEOMETRYSHADER_H__
#define __YAVIN_GEOMETRYSHADER_H__

#include <string>

#include "ShaderStage.h"

namespace Yavin {
class GeometryShader : public ShaderStage {
 public:
  GeometryShader(const std::string& filename);
  GeometryShader(GeometryShader&& other);
};
}  // namespace Yavin

#endif