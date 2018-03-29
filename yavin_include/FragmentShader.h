#ifndef __YAVIN_FRAGMENTSHADER_H__
#define __YAVIN_FRAGMENTSHADER_H__

#include <string>
#include "gl_includes.h"

#include "ShaderStage.h"
#include "dll_export.h"

namespace Yavin {
class FragmentShader : public ShaderStage {
 public:
  DLL_API FragmentShader(const std::string& filename);
  DLL_API FragmentShader(FragmentShader&& other);
};
}  // namespace Yavin

#endif