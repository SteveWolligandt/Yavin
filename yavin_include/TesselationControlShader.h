#ifndef __YAVIN_TESSELATIONCONTROLSHADER_H__
#define __YAVIN_TESSELATIONCONTROLSHADER_H__

#include <string>

#include "ShaderStage.h"
#include "dll_export.h"

namespace Yavin {
class TesselationControlShader : public ShaderStage {
 public:
  DLL_API TesselationControlShader(const std::string& filename);
  DLL_API TesselationControlShader(TesselationControlShader&& other);
};
}  // namespace Yavin

#endif