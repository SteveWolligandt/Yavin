#ifndef __YAVIN_COMPUTESHADER_H__
#define __YAVIN_COMPUTESHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class ComputeShader : public ShaderStage {
 public:
  DLL_API ComputeShader(const std::string& filename);
  DLL_API ComputeShader(ComputeShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
