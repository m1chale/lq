#pragma once

#include <filesystem>
#include <string_view>

namespace lq::consts {

// Logseq layout / naming
inline constexpr std::string_view pagesDirectory = "pages";
inline constexpr std::string_view journalsDirectory = "journals";
inline constexpr std::string_view assetDirectory = "assets";

// File conventions
inline constexpr std::string_view markdownExtension = ".md";
inline constexpr std::string_view ignorePagePrefix = "hls__";

} // namespace lq::consts