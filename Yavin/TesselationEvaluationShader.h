#ifndef __YAVIN_TESSELATIONEVALUATIONSHADER_H__
#define __YAVIN_TESSELATIONEVALUATIONSHADER_H__

#include <string>

#include "ShaderStage.h"

namespace Yavin {
class TesselationEvaluationShader : public ShaderStage {
 public:
  TesselationEvaluationShader(const std::string& filename);
  TesselationEvaluationShader(TesselationEvaluationShader&& other);
};
}  // namespace Yavin

#endif