#ifndef YAVIN_SHADERSTAGEPARSER_H
#define YAVIN_SHADERSTAGEPARSER_H
//==============================================================================
#include <exception>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>

#include "dllexport.h"
#include "glslvar.h"
#include "includetree.h"
//==============================================================================
namespace yavin {
//==============================================================================
enum shadersourcetype { FILE, SOURCE };
class shaderstageparser {
 public:
  DLL_API static std::string parse(std::string const&    filename_or_source,
                                   std::vector<GLSLVar>& vars, include_tree& it,
                                   shadersourcetype string_type = FILE);
  DLL_API static std::string parse_file(
      std::filesystem::path const& filename_or_source,
      std::vector<GLSLVar>& vars, include_tree& it);
  DLL_API static std::string parse_source(std::string const& filename_or_source,
                                          std::vector<GLSLVar>& vars,
                                          include_tree&         it);

  DLL_API static std::optional<GLSLVar> parse_varname(std::string const& line);
  DLL_API static std::optional<std::string> parse_include(
      std::string const& line);

 private:
  template <typename Stream>
  static std::string parse_stream(Stream& stream, std::vector<GLSLVar>& vars,
                                  include_tree&      it,
                                  std::string const& folder = "") {
    std::string line;
    std::string content;

    int line_number = 0;

    while (std::getline(stream, line)) {
      if (auto parsed_var = parse_varname(line); parsed_var)
        vars.push_back(parsed_var.value());

      if (auto parsed_include = parse_include(line); parsed_include) {
        it.nested_include_trees().emplace_back(line_number, 0,
                                               parsed_include.value(), it);
        content += parse_file(folder + parsed_include.value(), vars,
                              it.nested_include_trees().back());
      } else
        content += line + '\n';

      ++line_number;
    }
    it.num_lines() = line_number;

    return content;
  }

  static std::regex const regex_var;
  static std::regex const regex_include;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
