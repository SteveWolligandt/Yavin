#ifndef __YAVIN_ANSI_FORMAT_H__
#define __YAVIN_ANSI_FORMAT_H__

#include <string>
//==============================================================================
namespace yavin::ansi {
//==============================================================================

static std::string black   = "\033[30m";
static std::string red     = "\033[31m";
static std::string green   = "\033[32m";
static std::string yellow  = "\033[33m";
static std::string blue    = "\033[34m";
static std::string magenta = "\033[35m";
static std::string cyan    = "\033[36m";
static std::string white   = "\033[37m";

static std::string black_bg   = "\033[40m";
static std::string red_bg     = "\033[41m";
static std::string green_bg   = "\033[42m";
static std::string yellow_bg  = "\033[43m";
static std::string blue_bg    = "\033[44m";
static std::string magenta_bg = "\033[45m";
static std::string cyan_bg    = "\033[46m";
static std::string white_bg   = "\033[47m";

static std::string reset         = "\033[0m";
static std::string bold          = "\033[1m";
static std::string underline     = "\033[4m";
static std::string inverse       = "\033[7m";
static std::string bold_off      = "\033[21m";
static std::string underline_off = "\033[24m";
static std::string inverse_off   = "\033[27m";

//==============================================================================
}  // namespace yavin::ansi
//==============================================================================

#endif
