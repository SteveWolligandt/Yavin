#ifndef __YAVIN_INCLUDE_TREE_H__
#define __YAVIN_INCLUDE_TREE_H__

#include <iostream>
#include <list>
#include <string>

//==============================================================================
namespace Yavin {
//==============================================================================

struct IncludeTree {
  int                    line_number;
  size_t                 num_lines;
  std::string            file_name;
  std::list<IncludeTree> nested_include_trees;
  IncludeTree*           parent;

  //! returns file and line number
  std::pair<const IncludeTree*, size_t> parse_line(size_t n) const {
    size_t cur_offset = 0;
    for (const auto& nesting : nested_include_trees) {
      if (n >= nesting.line_number + cur_offset &&
          n < nesting.line_number + cur_offset +
                  nesting.num_lines_with_includes())
        return nesting.parse_line(n - cur_offset - nesting.line_number);
      else
        cur_offset += nesting.num_lines_with_includes() - 1;
    }
    return {this, n - cur_offset};
  }

  size_t num_lines_with_includes() const {
    size_t n = num_lines;
    n -= nested_include_trees.size();
    for (const auto& nesting : nested_include_trees)
      n += nesting.num_lines_with_includes();
    return n;
  }

  void print(size_t indent = 0) const {
    for (size_t i = 0; i < indent; ++i) std::cout << ' ';
    std::cout << (line_number >= 0 ? (std::to_string(line_number) + "/" +
                                      std::to_string(num_lines) + ": ")
                                   : "")
              << file_name << '\n';
    for (const auto& nesting : nested_include_trees) nesting.print(indent + 2);
  }
};

//==============================================================================
}  // namespace Yavin
//==============================================================================

#endif
