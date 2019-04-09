#ifndef __YAVIN_TESSELATIONEVALUATIONSHADER_H__
#define __YAVIN_TESSELATIONEVALUATIONSHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class tesselationevaluationshader : public shaderstage {
 public:
  DLL_API tesselationevaluationshader(const std::string& filename);
  DLL_API tesselationevaluationshader(tesselationevaluationshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
