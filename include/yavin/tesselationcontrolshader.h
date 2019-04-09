#ifndef __YAVIN_TESSELATIONCONTROLSHADER_H__
#define __YAVIN_TESSELATIONCONTROLSHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class TesselationControlShader : public ShaderStage {
 public:
  DLL_API TesselationControlShader(const std::string& filename);
  DLL_API TesselationControlShader(TesselationControlShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
