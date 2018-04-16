#ifndef __YAVIN_SHADERSTAGE_H__
#define __YAVIN_SHADERSTAGE_H__

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include "ShaderStageParser.h"
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class ShaderStage {
 public:
  using StringType = ShaderStageParser::StringType;

  DLL_API ShaderStage(GLenum shader_type, const std::string& shaderfilepath,
                      StringType string_type = StringType::FILE);
  DLL_API ShaderStage(ShaderStage&& other);
  DLL_API ~ShaderStage();
  DLL_API const unsigned int& id() const;
  DLL_API std::string         stage();
  const std::vector<GLSLVar>& glsl_vars() const { return m_glsl_vars; }
  DLL_API static std::string  type_to_string(GLenum shader_type);

  static const std::regex regex_compiler;

 protected:
  DLL_API void print_log();

  GLuint m_id = 0;

  bool                          dont_delete = false;
  GLenum                        m_shader_type;
  ShaderStageParser::StringType m_string_type;
  std::string                   m_filename_or_source;

  std::vector<GLSLVar> m_glsl_vars;
  IncludeTree          m_include_tree;
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
