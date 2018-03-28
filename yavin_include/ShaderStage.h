#ifndef __YAVIN_SHADERSTAGE_H__
#define __YAVIN_SHADERSTAGE_H__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "ShaderStageParser.h"
#include "error_check.h"
#include "gl_includes.h"

namespace Yavin {

class ShaderStage {
 public:
  ShaderStage(GLenum shaderType, const std::string& shaderfilepath);
  ShaderStage(ShaderStage&& other);
  ~ShaderStage();
  const unsigned int& id() const;

  const std::string& stage() { return m_shader_type_name; }

  const std::vector<GLSLVar>& glsl_vars() const { return m_glsl_vars; }

 protected:
  void         print_log();
  unsigned int m_id = 0;

  bool        dont_delete = false;
  std::string m_shader_type_name;

  std::vector<GLSLVar> m_glsl_vars;
};

}  // namespace Yavin

#endif