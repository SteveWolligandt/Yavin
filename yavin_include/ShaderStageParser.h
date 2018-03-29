#ifndef __YAVIN_SHADERSTAGEPARSER_H__
#define __YAVIN_SHADERSTAGEPARSER_H__

#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include "GLSLVar.h"
#include "dll_export.h"

namespace Yavin {
class ShaderStageParser {
 public:
  DLL_API static std::string parse(const std::string& filename, std::vector<GLSLVar>& vars);

  DLL_API static std::optional<GLSLVar> parse_varname(const std::string& line);
  DLL_API static std::optional<std::string> parse_include(const std::string& line);

 private:
  static const std::regex regex_var;
  static const std::regex regex_include;
};

}  // namespace Yavin

#endif