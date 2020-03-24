#ifndef YAVIN_FRAGMENTSHADER_H
#define YAVIN_FRAGMENTSHADER_H

#include <string>
#include "glincludes.h"

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class fragmentshader : public shaderstage {
 public:
  using StringType = shaderstage::StringType;
  DLL_API fragmentshader(const std::string& filename,
                         StringType         string_type = StringType::FILE);
  DLL_API fragmentshader(fragmentshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
