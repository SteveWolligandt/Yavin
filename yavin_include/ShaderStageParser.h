#ifndef __YAVIN_SHADERSTAGEPARSER_H__
#define __YAVIN_SHADERSTAGEPARSER_H__

#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include "GLSLVar.h"

namespace Yavin {
class ShaderStageParser {
 public:
  static std::string parse(const std::string& filename, std::vector<GLSLVar>& vars);

  static std::optional<GLSLVar> parse_varname(const std::string& line);
  static std::optional<std::string> parse_include(const std::string& line);

 private:
  static const std::regex regex_var;
  static const std::regex regex_include;
};

}  // namespace Yavin

#endif