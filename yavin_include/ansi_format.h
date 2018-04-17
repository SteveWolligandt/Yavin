#ifndef __YAVIN_ANSI_FORMAT_H__
#define __YAVIN_ANSI_FORMAT_H__

#include <string>
//==============================================================================
namespace Yavin::ansi {
//==============================================================================

std::string black   = "\033[30m";
std::string red     = "\033[31m";
std::string green   = "\033[32m";
std::string yellow  = "\033[33m";
std::string blue    = "\033[34m";
std::string magenta = "\033[35m";
std::string cyan    = "\033[36m";
std::string white   = "\033[37m";

std::string black_bg   = "\033[40m";
std::string red_bg     = "\033[41m";
std::string green_bg   = "\033[42m";
std::string yellow_bg  = "\033[43m";
std::string blue_bg    = "\033[44m";
std::string magenta_bg = "\033[45m";
std::string cyan_bg    = "\033[46m";
std::string white_bg   = "\033[47m";

std::string reset         = "\033[0m";
std::string bold          = "\033[1m";
std::string underline     = "\033[4m";
std::string inverse       = "\033[7m";
std::string bold_off      = "\033[21m";
std::string underline_off = "\033[24m";
std::string inverse_off   = "\033[27m";

//==============================================================================
}  // namespace Yavin::ansi
//==============================================================================

#endif
