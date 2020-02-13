#ifndef __YAVIN_GEOMETRYSHADER_H__
#define __YAVIN_GEOMETRYSHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class geometryshader : public shaderstage {
 public:
  using StringType = shaderstage::StringType;
  DLL_API geometryshader(const std::string& filename,
                         StringType         string_type = StringType::FILE);
  DLL_API geometryshader(geometryshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
