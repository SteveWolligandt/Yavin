#include "ShaderStage.h"
#include "ansi_format.h"
#include "gl_includes.h"
#include "shader_include_paths.h"

//==============================================================================
namespace Yavin {
//==============================================================================

const std::regex ShaderStage::regex_compiler(
    R"(\d+\((\d+)\)\s*:\s*(error|warning)\s*\w*:\s*(.*))");

ShaderStage::ShaderStage(GLenum             shader_type,
                         const std::string &filename_or_source,
                         StringType         string_type)
    : m_id(glCreateShader(shader_type)),
      m_shader_type(shader_type),
      m_string_type(string_type),
      m_filename_or_source(filename_or_source),
      m_include_tree{-1, 0, "", {}, nullptr} {
  gl_error_check("glCreateShader");

  auto  source   = ShaderStageParser::parse(filename_or_source, m_glsl_vars,
                                         m_include_tree, m_string_type);
  auto *source_c = source.c_str();
  glShaderSource(m_id, 1, &source_c, nullptr);
  gl_error_check("glShaderSource");

  // Compile Shader
  glCompileShader(id());
  gl_error_check("glCompileShader");
  print_log();
}

ShaderStage::ShaderStage(ShaderStage &&other)
    : m_id(other.m_id), m_glsl_vars(std::move(other.m_glsl_vars)) {
  other.dont_delete = true;
}

ShaderStage::~ShaderStage() {
  if (!dont_delete) glDeleteShader(m_id);
}

const GLuint &ShaderStage::id() const { return m_id; }

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

std::string ShaderStage::stage() { return type_to_string(m_shader_type); }

void ShaderStage::print_log() {
  GLint   info_log_length = 0;
  GLsizei chars_written   = 0;

  glGetShaderiv(id(), GL_INFO_LOG_LENGTH, &info_log_length);
  gl_error_check("glGetShaderiv");

  if (info_log_length > 0) {
    auto log = new char[info_log_length];

    glGetShaderInfoLog(m_id, info_log_length, &chars_written, log);
    gl_error_check("glGetShaderInfoLog");

    std::istringstream is(log);
    std::ostringstream os;
    delete[] log;

    std::string line;
    while (std::getline(is, line)) {
      std::smatch match;
      std::regex_match(line, match, regex_compiler);

      if (match.str(1).size() > 0) {
        size_t line_number = stoul(match.str(1));

        auto [include_tree_ptr, error_line] =
            m_include_tree.parse_line(line_number - 1);
        // print file and include hierarchy
        os << ansi::red_bold << "[GLSL " << stage() << " Shader "
           << match.str(2) << "]" << ansi::reset << "\n  in file " << ansi::bold
           << include_tree_ptr->file_name << ":" << error_line + 1
           << ansi::reset << ": " << match.str(3) << '\n';
        auto hierarchy = include_tree_ptr;
        while (hierarchy->parent) {
          os << "    included from " << hierarchy->parent->file_name
             << " in line " << hierarchy->line_number << '\n';
          hierarchy = hierarchy->parent;
        }

        // print actual error message

        // print line that produces the error
        std::ifstream file(include_tree_ptr->file_name);
        if (!file.is_open())
          file.open(shader_dir + include_tree_ptr->file_name);
        if (file) {
          std::string file_line;
          size_t      line_cnt = 0;
          while (std::getline(file, file_line)) {
            if (line_cnt == error_line) {
              os << "  " << file_line << '\n';
              os << "  " << ansi::bold;
              for (size_t i = 0; i < file_line.size(); ++i) os << '~';
              os << ansi::reset;
              break;
            }
            ++line_cnt;
          }
          file.close();
        }
      }

      throw std::runtime_error(os.str());
    }
  }
}
//==============================================================================
}  // namespace Yavin
//==============================================================================
