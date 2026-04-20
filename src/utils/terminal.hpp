#pragma once

#include <iostream>
#include <string>

namespace lq::term {
// ANSI SGR codes
inline constexpr const char *resetHard = "\033[0m";
inline constexpr const char *fg_reset = "\033[39m";
inline constexpr const char *bg_reset = "\033[49m";

inline constexpr const char *bold_on = "\033[1m";
inline constexpr const char *bold_off = "\033[22m";

inline constexpr const char *italic_on = "\033[3m";
inline constexpr const char *italic_off = "\033[23m";
inline constexpr const char *underline_on = "\033[4m";
inline constexpr const char *underline_off = "\033[24m";

// colors
inline constexpr const char *red = "\033[31m";
inline constexpr const char *green = "\033[32m";
inline constexpr const char *yellow = "\033[33m";
inline constexpr const char *blue = "\033[34m";

// small helpers
inline std::string make_bold(const std::string &s) { return std::string(bold_on) + s + std::string(bold_off); }
} // namespace lq::term