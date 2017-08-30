#ifndef __YAVIN_FRAGMENTSHADER_H__
#define __YAVIN_FRAGMENTSHADER_H__

#include <string>
#include "gl_includes.h"

#include "ShaderStage.h"

namespace Yavin {
class FragmentShader : public ShaderStage {
 public:
  FragmentShader(const std::string& filename);
  FragmentShader(FragmentShader&& other);
};
}  // namespace Yavin

#endif