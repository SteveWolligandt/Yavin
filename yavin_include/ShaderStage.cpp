#include <memory>

#include "ShaderStage.h"
#include "ansi_format.h"
#include "gl_includes.h"
#include "shader_include_paths.h"

//==============================================================================
namespace Yavin {
//==============================================================================

const std::regex ShaderStage::regex_nvidia_compiler_error(
    R"(\d+\((\d+)\)\s*:\s*(error|warning)\s*\w*:\s*(.*))");

//------------------------------------------------------------------------------

const std::regex ShaderStage::regex_mesa_compiler_error(
    R"(\d+:(\d+)\(\d+\)\s*:\s*(error|warning)\s*\w*:\s*(.*))");

//==============================================================================

ShaderStage::ShaderStage(GLenum             shader_type,
                         const std::string &filename_or_source,
                         StringType string_type, bool use_ansi_color)
    : m_shader_type(shader_type),
      m_string_type(string_type),
      m_filename_or_source(filename_or_source),
      m_include_tree{-1, 0, "", {}, nullptr} {}

//------------------------------------------------------------------------------

ShaderStage::ShaderStage(ShaderStage &&other)
    : m_id(other.m_id),
      m_shader_type(other.m_shader_type),
      m_string_type(other.m_string_type),
      m_filename_or_source(std::move(other.m_filename_or_source)),
      m_glsl_vars(std::move(other.m_glsl_vars)),
      m_include_tree(std::move(other.m_include_tree)) {
  other.m_delete = false;
}

//------------------------------------------------------------------------------

ShaderStage::~ShaderStage() {
  if (m_delete) delete_stage();
}

//------------------------------------------------------------------------------

std::string ShaderStage::type_to_string(GLenum shader_type) {
  switch (shader_type) {
    case GL_VERTEX_SHADER: return "Vertex";
    case GL_FRAGMENT_SHADER: return "Fragment";
    case GL_GEOMETRY_SHADER: return "Geometry";
    case GL_TESS_EVALUATION_SHADER: return "Tesselation Evaluation";
    case GL_TESS_CONTROL_SHADER: return "Tesselation Control";
    case GL_COMPUTE_SHADER: return "Compute";
    default: return "unknown";
  }
}

//------------------------------------------------------------------------------

void ShaderStage::compile(bool use_ansi_color) {
  delete_stage();
  m_id = glCreateShader(m_shader_type);
  gl_error_check("glCreateShader");
  auto  source   = ShaderStageParser::parse(m_filename_or_source, m_glsl_vars,
                                         m_include_tree, m_string_type);
  auto *source_c = source.c_str();
  glShaderSource(m_id, 1, &source_c, nullptr);
  gl_error_check("glShaderSource");

  // Compile Shader
  glCompileShader(id());
  gl_error_check("glCompileShader");
  info_log(use_ansi_color);
}

//------------------------------------------------------------------------------

void ShaderStage::delete_stage() {
  if (m_id) {
    glDeleteShader(m_id);
    gl_error_check("glDeleteShader");
  }
  m_id = 0;
}

//------------------------------------------------------------------------------

void ShaderStage::info_log(bool use_ansi_color) {
  GLint   info_log_length = 0;
  GLsizei chars_written   = 0;

  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &info_log_length);
  gl_error_check("glGetShaderiv");

  if (info_log_length > 0) {
    auto log = new char[info_log_length];

    glGetShaderInfoLog(m_id, info_log_length, &chars_written, log);
    gl_error_check("glGetShaderInfoLog");

    std::istringstream is(log);
    delete[] log;
    std::ostringstream os;

    std::string line;
    while (std::getline(is, line)) {
      std::smatch match;

      std::regex_match(line, match, regex_nvidia_compiler_error);
      if (!match.str(0).empty()) {
        parse_compile_error(match, os, use_ansi_color);
        os << '\n';
        continue;
      }
      std::regex_match(line, match, regex_mesa_compiler_error);
      if (!match.str(0).empty()) {
        parse_compile_error(match, os, use_ansi_color);
        os << '\n';
        continue;
      }
      os << line << '\n';
    }
    throw std::runtime_error(os.str());
  }
}

//------------------------------------------------------------------------------

void ShaderStage::parse_compile_error(std::smatch &match, std::ostream &os,
                                      bool use_ansi_color) {
  const size_t line_number = stoul(match.str(1));
  const auto [include_tree_ptr, error_line] =
      m_include_tree.parse_line(line_number - 1);

  // print file and include hierarchy
  if (use_ansi_color) os << ansi::red << ansi::bold;
  os << "[GLSL " << stage_name() << " Shader " << match.str(2) << "]\n";
  if (use_ansi_color) os << ansi::reset;

  os << "in file ";
  if (use_ansi_color) os << ansi::cyan << ansi::bold;
  os << include_tree_ptr->filename << ":";

  if (use_ansi_color) os << ansi::yellow << ansi::bold;
  os << error_line + 1;
  if (use_ansi_color) os << ansi::reset;
  os << ": " << match.str(3) << '\n';

  auto hierarchy = include_tree_ptr;
  while (hierarchy->parent) {
    os << "    included from ";
    if (use_ansi_color) os << ansi::bold;
    os << hierarchy->parent->filename;

    if (use_ansi_color) os << ansi::reset;
    os << ":";
    if (use_ansi_color) os << ansi::bold;
    os << hierarchy->line_number << '\n';
    if (use_ansi_color) os << ansi::reset;
    hierarchy = hierarchy->parent;
  }

  print_line(include_tree_ptr->filename, error_line, os);
}

//------------------------------------------------------------------------------

void ShaderStage::print_line(const std::string &filename, size_t line_number,
                             std::ostream &os) {
  std::ifstream file(filename);
  if (!file.is_open()) file.open(shader_dir + filename);

  if (file.is_open()) {
    std::string line;
    size_t      line_cnt = 0;
    while (std::getline(file, line)) {
      if (line_cnt == line_number) {
        os << "  " << line << "\n  ";
        os << ansi::bold;
        for (size_t i = 0; i < line.size(); ++i) os << '~';
        os << ansi::reset;
        break;
      }
      ++line_cnt;
    }
    file.close();
  }
}

//==============================================================================
}  // namespace Yavin
//==============================================================================
