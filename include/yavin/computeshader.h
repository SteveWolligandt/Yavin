#ifndef __YAVIN_COMPUTESHADER_H__
#define __YAVIN_COMPUTESHADER_H__

#include <string>

#include "shaderstage.h"
#include "dllexport.h"

//==============================================================================
namespace yavin {
//==============================================================================

class computeshader : public shaderstage {
 public:
  using StringType = shaderstage::StringType;
  DLL_API computeshader(const std::string& filename,
                         StringType         string_type = StringType::FILE);
  DLL_API computeshader(computeshader&& other);
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
