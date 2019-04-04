#ifndef __YAVIN_SHADERSTAGE_H__
#define __YAVIN_SHADERSTAGE_H__

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
#include "dll_export.h"
#include "error_check.h"
#include "gl_includes.h"
#include "shader_stage_parser.h"

//==============================================================================
namespace yavin {
//==============================================================================

class ShaderStage {
 public:
  using StringType = ShaderStageParser::StringType;
  DLL_API static std::string type_to_string(GLenum shader_type);

  //----------------------------------------------------------------------------

  DLL_API ShaderStage(GLenum shader_type, const std::string& shaderfilepath,
                      StringType string_type    = StringType::FILE);

  //----------------------------------------------------------------------------

  DLL_API ShaderStage(ShaderStage&& other);

  //----------------------------------------------------------------------------

  DLL_API ~ShaderStage();

  //----------------------------------------------------------------------------

  DLL_API void compile(bool use_ansi_color = true);
  DLL_API void delete_stage();

  //----------------------------------------------------------------------------

  const auto& glsl_vars() const { return m_glsl_vars; }

  //----------------------------------------------------------------------------

  auto id() const { return m_id; }
  auto stage_name() const { return type_to_string(m_shader_type); }
  auto stage_type() const { return m_shader_type; }
  bool is_created() const { return m_id != 0; }

 protected:
  DLL_API void info_log(bool use_ansi_color = true);
  DLL_API void parse_compile_error(std::smatch& match, std::ostream& os,
                                   bool use_ansi_color = true);
  DLL_API void print_line(const std::string& filename, size_t line_number,
                          std::ostream& os);

  GLuint m_id     = 0;
  bool   m_delete = true;

  GLenum                        m_shader_type;
  ShaderStageParser::StringType m_string_type;
  std::string                   m_filename_or_source;

  std::vector<GLSLVar> m_glsl_vars;
  IncludeTree          m_include_tree;

  static const std::regex regex_nvidia_compiler_error;
  static const std::regex regex_mesa_compiler_error;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
