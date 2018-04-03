#include "ShaderStageParser.h"

namespace Yavin {

/**
 * layout (location=<digit>)                   <- optional layout position
 * {in|out|uniform}                            <- modifier
 * {float | int | sampler<digit>D | mat<digit> | vec<digit>} <- datatype
 * <any>                                       <- variable name
 */
const std::regex ShaderStageParser::regex_var(
    R"((layout\s*\(location\s*=\s*\d+\s*\))?\s*(in|out|uniform)\s(float|int|uint|sampler\dD|mat\d|vec\d)\s(.*)[;])");

/**
 * #include <- keyword
 * "        <- string start
 * <any>    <- path
 * "        <- string end
 */
const std::regex ShaderStageParser::regex_include(
    R"(#include\s+\"(.*)\")");

std::string ShaderStageParser::parse(const std::string&    filename,
                                     std::vector<GLSLVar>& vars) {
  std::string fileContent;
  std::string line;
  std::string folder = filename.substr(0, filename.find_last_of("/\\") + 1);

  std::ifstream file(filename.c_str());

  if (!file.is_open())
    throw std::runtime_error("ERROR: Unable to open file " + filename);

  while (!file.eof()) {
    getline(file, line);
    auto parsed_var = parse_varname(line);
    if (parsed_var) vars.push_back(parsed_var.value());

    auto parsed_include = parse_include(line);
    if (parsed_include)
      fileContent += parse(folder + parsed_include.value(), vars);
    else
      fileContent += '\n' + line;
  }
  file.close();
  return fileContent;
}

std::optional<GLSLVar> ShaderStageParser::parse_varname(
    const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_var);

  if (match.str(4).size() > 0) {
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

std::optional<std::string> ShaderStageParser::parse_include(
    const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_include);

  if (match.str(1).size() > 0)
    return match.str(1);
  else
    return {};
}
}  // namespace Yavin
