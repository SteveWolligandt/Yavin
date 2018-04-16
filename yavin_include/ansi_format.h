#ifndef __YAVIN_ANSI_FORMAT_H__
#define __YAVIN_ANSI_FORMAT_H__

#include <string>
namespace Yavin::ansi {

std::string red_bold = "\033[1;31m";
std::string bold     = "\033[1m";
std::string reset    = "\033[0m";
}  // namespace Yavin::ansi

#endif
