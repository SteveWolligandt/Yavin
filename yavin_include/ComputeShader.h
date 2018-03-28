#ifndef __YAVIN_COMPUTESHADER_H__
#define __YAVIN_COMPUTESHADER_H__

#include <string>

#include "ShaderStage.h"

namespace Yavin {
class ComputeShader : public ShaderStage {
 public:
  ComputeShader(const std::string& filename);
  ComputeShader(ComputeShader&& other);
};
}  // namespace Yavin

#endif