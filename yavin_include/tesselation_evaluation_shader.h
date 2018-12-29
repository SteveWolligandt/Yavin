#ifndef __YAVIN_TESSELATIONEVALUATIONSHADER_H__
#define __YAVIN_TESSELATIONEVALUATIONSHADER_H__

#include <string>

#include "shader_stage.h"
#include "dll_export.h"

//==============================================================================
namespace yavin {
//==============================================================================

class TesselationEvaluationShader : public ShaderStage {
 public:
  DLL_API TesselationEvaluationShader(const std::string& filename);
  DLL_API TesselationEvaluationShader(TesselationEvaluationShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
