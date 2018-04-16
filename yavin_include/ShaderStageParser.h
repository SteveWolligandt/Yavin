#ifndef __YAVIN_SHADERSTAGEPARSER_H__
#define __YAVIN_SHADERSTAGEPARSER_H__

#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include "GLSLVar.h"
#include "dll_export.h"

//==============================================================================
namespace Yavin {
//==============================================================================

class ShaderStageParser {
 public:
  enum StringType { FILE, SOURCE };
  DLL_API static std::string parse(const std::string&    filename_or_source,
                                   std::vector<GLSLVar>& vars,
                                   StringType            string_type = FILE);
  DLL_API static std::string parse_file(const std::string& filename_or_source,
                                        std::vector<GLSLVar>& vars);
  DLL_API static std::string parse_source(const std::string& filename_or_source,
                                          std::vector<GLSLVar>& vars);

  DLL_API static std::optional<GLSLVar> parse_varname(const std::string& line);
  DLL_API static std::optional<std::string> parse_include(
      const std::string& line);

 private:
  template <typename Stream>
  static std::string parse_stream(Stream& stream, std::vector<GLSLVar>& vars,
                                  const std::string& folder = "") {
    std::string line;
    std::string content;

    while (std::getline(stream, line)) {
      auto parsed_var = parse_varname(line);
      if (parsed_var) vars.push_back(parsed_var.value());

      auto parsed_include = parse_include(line);
      if (parsed_include)
        content += parse(folder + parsed_include.value(), vars);
      else
        content += '\n' + line;
    }

    return content;
  }

  static const std::regex regex_var;
  static const std::regex regex_include;
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
