#ifndef __YAVIN_FRAGMENTSHADER_H__
#define __YAVIN_FRAGMENTSHADER_H__

#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class fragmentshader : public shaderstage {
 public:
  DLL_API fragmentshader(const std::string& filename);
  DLL_API fragmentshader(fragmentshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
