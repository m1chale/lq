#pragma once

#include <iostream>
#include <string>

namespace lq::term {
// ANSI SGR codes
inline constexpr const char *reset = "\033[0m";
inline constexpr const char *bold = "\033[1m";
inline constexpr const char *italic = "\033[3m";
inline constexpr const char *underline = "\033[4m";

// colors
inline constexpr const char *red = "\033[31m";
inline constexpr const char *green = "\033[32m";
inline constexpr const char *yellow = "\033[33m";
inline constexpr const char *blue = "\033[34m";

// small helpers
inline void print_bold(const std::string &s) { std::cout << bold << s << reset; }
}