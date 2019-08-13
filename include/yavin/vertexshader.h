#ifndef __YAVIN_VERTEXSHADER_H__
#define __YAVIN_VERTEXSHADER_H__

#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class vertexshader : public shaderstage {
 public:
  using StringType = shaderstage::StringType;
  DLL_API vertexshader(const std::string& filename,
                         StringType         string_type = StringType::FILE);
  DLL_API vertexshader(vertexshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
