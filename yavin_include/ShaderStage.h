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
                      StringType string_type    = StringType::FILE,
                      bool       use_ansi_color = true);
  DLL_API ShaderStage(ShaderStage&& other);
  DLL_API ~ShaderStage();
  const std::vector<GLSLVar>& glsl_vars() const { return m_glsl_vars; }
  DLL_API static std::string  type_to_string(GLenum shader_type);

  auto        id() const { return m_id; }
  std::string stage() const { return type_to_string(m_shader_type); }

 protected:
  DLL_API void print_log(bool use_ansi_color);
  DLL_API void parse_compile_error(std::smatch& match, std::ostream& os,
                                   bool use_ansi_color);
  DLL_API void print_line(const std::string& filename, size_t line_number,
                          std::ostream& os);
  GLuint       m_id = 0;

  bool                          dont_delete = false;
  GLenum                        m_shader_type;
  ShaderStageParser::StringType m_string_type;
  std::string                   m_filename_or_source;

  std::vector<GLSLVar> m_glsl_vars;
  IncludeTree          m_include_tree;

  static const std::regex regex_nvidia_compiler_error;
  static const std::regex regex_mesa_compiler_error;
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
