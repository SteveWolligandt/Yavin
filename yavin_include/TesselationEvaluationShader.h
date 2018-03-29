#ifndef __YAVIN_TESSELATIONEVALUATIONSHADER_H__
#define __YAVIN_TESSELATIONEVALUATIONSHADER_H__

#include <string>

#include "ShaderStage.h"
#include "dll_export.h"

namespace Yavin {
class TesselationEvaluationShader : public ShaderStage {
 public:
  DLL_API TesselationEvaluationShader(const std::string& filename);
  DLL_API TesselationEvaluationShader(TesselationEvaluationShader&& other);
};
}  // namespace Yavin

#endif