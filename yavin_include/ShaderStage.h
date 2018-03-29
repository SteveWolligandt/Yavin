#ifndef __YAVIN_SHADERSTAGE_H__
#define __YAVIN_SHADERSTAGE_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ShaderStageParser.h"
#include "error_check.h"
#include "gl_includes.h"
#include "dll_export.h"

namespace Yavin {

class ShaderStage {
 public:
  DLL_API ShaderStage(GLenum shaderType, const std::string& shaderfilepath);
  DLL_API ShaderStage(ShaderStage&& other);
  DLL_API ~ShaderStage();
  DLL_API const unsigned int& id() const;

  DLL_API const std::string& stage() { return m_shader_type_name; }

  DLL_API const std::vector<GLSLVar>& glsl_vars() const { return m_glsl_vars; }

 protected:
  DLL_API void         print_log();
  unsigned int m_id = 0;

  bool        dont_delete = false;
  std::string m_shader_type_name;

  std::vector<GLSLVar> m_glsl_vars;
};

}  // namespace Yavin

#endif