#ifndef YAVIN_GEOMETRYSHADER_H
#define YAVIN_GEOMETRYSHADER_H

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class geometryshader : public shaderstage {
 public:
  DLL_API geometryshader(const std::string& filename,
                         shadersourcetype string_type = FILE);
  DLL_API geometryshader(geometryshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
