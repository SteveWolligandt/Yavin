#include "shader_stage_parser.h"
#include "shader_include_paths.h"

#include <sstream>

//==============================================================================
namespace yavin {
//==============================================================================

const std::regex ShaderStageParser::regex_var(
    R"((layout\s*\(location\s*=\s*\d+\s*\))?\s*(in|out|uniform)\s(float|double|int|uint|bool|sampler\dD|mat\d|vec\d|ivec\d|uvec\d|bvec\d|dvec\d)\s(.*)[;])");

//------------------------------------------------------------------------------

const std::regex ShaderStageParser::regex_include(R"(#include\s+\"(.*)\")");

//==============================================================================

std::string ShaderStageParser::parse(const std::string&    filename_or_source,
                                     std::vector<GLSLVar>& vars,
                                     IncludeTree&          include_tree,
                                     StringType            string_type) {
  if (string_type == FILE)
    return parse_file(filename_or_source, vars, include_tree);
  else /*if (string_type == SOURCE)*/
    return parse_source(filename_or_source, vars, include_tree);
}

//------------------------------------------------------------------------------

std::string ShaderStageParser::parse_file(const std::string&    filename,
                                          std::vector<GLSLVar>& vars,
                                          IncludeTree&          include_tree) {
  include_tree.filename = filename;
  std::string   folder  = filename.substr(0, filename.find_last_of("/\\") + 1);
  std::ifstream file(filename);

  if (!file.is_open()) file.open(shader_dir + filename);
  if (!file.is_open())
    throw std::runtime_error("ERROR: Unable to open file " + filename);

  auto content = parse_stream(file, vars, include_tree, folder);
  file.close();
  return content;
}

//------------------------------------------------------------------------------

std::string ShaderStageParser::parse_source(const std::string&    source,
                                            std::vector<GLSLVar>& vars,
                                            IncludeTree& include_tree) {
  include_tree.filename = "from string";
  std::stringstream stream(source);
  return parse_stream(stream, vars, include_tree);
}

//------------------------------------------------------------------------------

std::optional<GLSLVar> ShaderStageParser::parse_varname(
    const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_var);

  if (!match.str(4).empty()) {
    return GLSLVar{[](const std::string& mod_name) {
                     if (mod_name == "uniform")
                       return GLSLVar::UNIFORM;
                     else if (mod_name == "in")
                       return GLSLVar::IN;
                     else if (mod_name == "out")
                       return GLSLVar::OUT;
                     else
                       return GLSLVar::UNKNOWN;
                   }(match.str(2)),
                   match.str(3), match.str(4)};
  } else
    return {};
}

//------------------------------------------------------------------------------

std::optional<std::string> ShaderStageParser::parse_include(
    const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_include);

  if (match.str(1).size() > 0)
    return match.str(1);
  else
    return {};
}

//==============================================================================
}  // namespace yavin
//==============================================================================
