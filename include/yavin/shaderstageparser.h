#ifndef __YAVIN_SHADERSTAGEPARSER_H__
#define __YAVIN_SHADERSTAGEPARSER_H__

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

class ShaderStageParser {
 public:
  enum StringType { FILE, SOURCE };
  DLL_API static std::string parse(const std::string&    filename_or_source,
                                   std::vector<GLSLVar>& vars,
                                   IncludeTree&          include_tree,
                                   StringType            string_type = FILE);
  DLL_API static std::string parse_file(const std::string& filename_or_source,
                                        std::vector<GLSLVar>& vars,
                                        IncludeTree&          include_tree);
  DLL_API static std::string parse_source(const std::string& filename_or_source,
                                          std::vector<GLSLVar>& vars,
                                          IncludeTree&          include_tree);

  DLL_API static std::optional<GLSLVar> parse_varname(const std::string& line);
  DLL_API static std::optional<std::string> parse_include(
      const std::string& line);

 private:
  template <typename Stream>
  static std::string parse_stream(Stream& stream, std::vector<GLSLVar>& vars,
                                  IncludeTree&       include_tree,
                                  const std::string& folder = "") {
    std::string line;
    std::string content;

    int line_number = 0;

    while (std::getline(stream, line)) {
      if (auto parsed_var = parse_varname(line); parsed_var)
        vars.push_back(parsed_var.value());

      if (auto parsed_include = parse_include(line); parsed_include) {
        include_tree.nested_include_trees.push_back(
            {line_number, 0, parsed_include.value(), {}, &include_tree});
        content += parse_file(folder + parsed_include.value(), vars,
                              include_tree.nested_include_trees.back());
      } else
        content += line + '\n';

      ++line_number;
    }
    include_tree.num_lines = line_number;

    return content;
  }

  static const std::regex regex_var;
  static const std::regex regex_include;
};

//==============================================================================
}  // namespace yavin
//==============================================================================

#endif
