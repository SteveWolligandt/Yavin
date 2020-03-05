#include <iostream>
#include <iterator>
#include <optional>
#include <regex>
#include <string>

struct GLSLVar {
  enum modifier_t { UNIFORM, IN, OUT, UNKNOWN };
  modifier_t  modifier;
  std::string datatype;
  std::string name;
};

std::string modifier_to_string(const GLSLVar::modifier_t& modifier) {
  switch (modifier) {
    case GLSLVar::UNIFORM:
      return "uniform";
    case GLSLVar::IN:
      return "in";
    case GLSLVar::OUT:
      return "out";
    default:
      return "";
  }
}

const std::regex regex_var(
    R"((layout\s*\(location\s*=\s*\d+\s*\))?\s*(in|out|uniform)\s(sampler\dD|mat\d|vec\d)\s(.*)[;])");

const std::regex regex_include(
    R"(#include\s+\"(.*)\")");

std::optional<GLSLVar> parse_varname(const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_var);

  if (match.str(7).size() > 0) {
    return GLSLVar{[](const std::string& mod_name) {
                     if (mod_name == "uniform")
                       return GLSLVar::UNIFORM;
                     else if (mod_name == "in")
                       return GLSLVar::IN;
                     else if (mod_name == "out")
                       return GLSLVar::OUT;
                     else
                       return GLSLVar::UNKNOWN;
                   }(match.str(5)),
                   match.str(6), match.str(7)};
  } else
    return {};
}

std::optional<std::string> parse_include(const std::string& line) {
  std::smatch match;

  std::regex_match(line, match, regex_include);

  if (match.str(1).size() > 0)
    return match.str(1);
  else
    return {};
}

void parse_and_dump(const std::string& line, const std::regex& regex) {
  std::smatch match;

  std::regex_match(line, match, regex);

  for (size_t i = 0; i < match.size(); ++i) std::cout << i << ": " << match.str(i) << '\n';
}

int main() {
  std::string line1 = "in vec3 foo;";
  std::string line2 = "layout (location=0) in vec3 bar;";
  std::string line3 = "uniform sampler2D bar;";
  std::string line4 = "#include \"dasdsa\"";

  parse_and_dump(line1, regex_var);
  parse_and_dump(line2, regex_var);

  // auto parsed_line1 = parse_varname(line1);
  // auto parsed_line2 = parse_varname(line2);
  // auto parsed_line3 = parse_varname(line3);
  // auto parsed_line4 = parse_include(line4);

  // if (parsed_line1)
  //   std::cout << "line1 has "
  //             << modifier_to_string(parsed_line1.value().modifier) << " var "
  //             << parsed_line1.value().name << " of type "
  //             << parsed_line1.value().datatype << '\n';
  // else
  //   std::cout << "line1 has no var declaration\n";

  // if (parsed_line2)
  //   std::cout << "line2 has "
  //             << modifier_to_string(parsed_line2.value().modifier) << " var "
  //             << parsed_line2.value().name << " of type "
  //             << parsed_line2.value().datatype << '\n';
  // else
  //   std::cout << "line2 has no var declaration\n";
  // //
  // if (parsed_line3)
  //   std::cout << "line3 has "
  //             << modifier_to_string(parsed_line3.value().modifier) << " var "
  //             << parsed_line3.value().name << " of type "
  //             << parsed_line3.value().datatype << '\n';
  // else
  //   std::cout << "line3 has no var declaration\n";

  // if (parsed_line4)
  //   std::cout << "line4 includes file " << parsed_line4.value() << '\n';
  // else
  //   std::cout << "line4 has no include directive\n";
}