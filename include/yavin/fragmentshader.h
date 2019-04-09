#ifndef __YAVIN_FRAGMENTSHADER_H__
#define __YAVIN_FRAGMENTSHADER_H__

#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class FragmentShader : public ShaderStage {
 public:
  DLL_API FragmentShader(const std::string& filename);
  DLL_API FragmentShader(FragmentShader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
