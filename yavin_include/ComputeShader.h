#ifndef __YAVIN_COMPUTESHADER_H__
#define __YAVIN_COMPUTESHADER_H__

#include <string>

#include "ShaderStage.h"
#include "dll_export.h"

namespace Yavin {
class ComputeShader : public ShaderStage {
 public:
  DLL_API ComputeShader(const std::string& filename);
  DLL_API ComputeShader(ComputeShader&& other);
};
}  // namespace Yavin

#endif