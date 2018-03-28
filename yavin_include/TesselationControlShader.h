#ifndef __YAVIN_TESSELATIONCONTROLSHADER_H__
#define __YAVIN_TESSELATIONCONTROLSHADER_H__

#include <string>

#include "ShaderStage.h"

namespace Yavin {
class TesselationControlShader : public ShaderStage {
 public:
  TesselationControlShader(const std::string& filename);
  TesselationControlShader(TesselationControlShader&& other);
};
}  // namespace Yavin

#endif