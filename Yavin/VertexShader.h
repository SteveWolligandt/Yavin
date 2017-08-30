#ifndef __YAVIN_VERTEXSHADER_H__
#define __YAVIN_VERTEXSHADER_H__

#include <string>
#include "gl_includes.h"

#include "ShaderStage.h"

namespace Yavin {
class VertexShader : public ShaderStage {
 public:
  VertexShader(const std::string& filename);
  VertexShader(VertexShader&& other);
};
}  // namespace Yavin

#endif