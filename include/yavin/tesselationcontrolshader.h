#ifndef __YAVIN_TESSELATIONCONTROLSHADER_H__
#define __YAVIN_TESSELATIONCONTROLSHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class tesselationcontrolshader : public shaderstage {
 public:
  DLL_API tesselationcontrolshader(const std::string& filename);
  DLL_API tesselationcontrolshader(tesselationcontrolshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
