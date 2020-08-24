#include <memory>

#include <yavin/ansiformat.h>
#include <yavin/glfunctions.h>
#include <yavin/shaderincludepaths.h>
#include <yavin/shaderstage.h>

//==============================================================================
namespace yavin {
//==============================================================================
const std::regex shaderstage::regex_nvidia_compiler_error(
    R"(\d+\((\d+)\)\s*:\s*(error|warning)\s*\w*:\s*(.*))");
//------------------------------------------------------------------------------
const std::regex shaderstage::regex_mesa_compiler_error(
    R"(\d+:(\d+)\(\d+\)\s*:\s*(error|warning)\s*\w*:\s*(.*))");
//==============================================================================
shaderstage::shaderstage(GLenum             shader_type,
                         const std::string &filename_or_source,
                         shadersourcetype string_type)
    : m_shader_type(shader_type),
      m_string_type(string_type),
      m_filename_or_source(filename_or_source),
      m_include_tree{-1, 0, "", {}, nullptr} {}
//------------------------------------------------------------------------------
shaderstage::shaderstage(shaderstage &&other)
    : m_id(other.m_id),
      m_shader_type(other.m_shader_type),
      m_string_type(other.m_string_type),
      m_filename_or_source(std::move(other.m_filename_or_source)),
      m_glsl_vars(std::move(other.m_glsl_vars)),
      m_include_tree(std::move(other.m_include_tree)) {
  other.m_delete = false;
}
//------------------------------------------------------------------------------
shaderstage::~shaderstage() {
  if (m_delete) delete_stage();
}
//------------------------------------------------------------------------------
std::string shaderstage::type_to_string(GLenum shader_type) {
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
void shaderstage::compile(bool use_ansi_color) {
  delete_stage();
  m_id           = gl::create_shader(m_shader_type);
  auto source    = shaderstageparser::parse(m_filename_or_source, m_glsl_vars,
                                         m_include_tree, m_string_type);
  auto source_c = source.c_str();
  gl::shader_source(m_id, 1, &source_c, nullptr);
  gl::compile_shader(m_id);
  info_log(use_ansi_color);
}
//------------------------------------------------------------------------------
void shaderstage::delete_stage() {
  if (m_id) gl::delete_shader(m_id);
  m_id = 0;
}
//------------------------------------------------------------------------------
void shaderstage::info_log(bool use_ansi_color) {
  auto info_log_length = gl::get_shader_info_log_length(m_id);
  if (info_log_length > 0) {
    auto info_log = gl::get_shader_info_log(m_id, info_log_length);
    std::istringstream is(info_log);
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
void shaderstage::parse_compile_error(std::smatch &match, std::ostream &os,
                                      bool use_ansi_color) {
  const size_t line_number = stoul(match.str(1));
  const auto [include_tree_ptr, error_line] =
      m_include_tree.parse_line(line_number - 1);

  // print file and include hierarchy
  if (use_ansi_color) { os << ansi::red << ansi::bold; }
  os << "[GLSL " << stage_name() << " Shader " << match.str(2) << "]\n";
  if (use_ansi_color) { os << ansi::reset; }

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
void shaderstage::print_line(const std::string &filename, size_t line_number,
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
}  // namespace yavin
//==============================================================================
