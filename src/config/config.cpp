#include "config.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <string_view>

#include "../utils/strings.hpp"

namespace fs = std::filesystem;

namespace {
inline constexpr std::string_view configFileName = ".lq";
inline constexpr std::string_view configDelimiter = "=";
inline constexpr std::string_view configLogseqPath = "LOGSEQ_PATH";
inline constexpr std::string_view configGraphName = "GRAPH_NAME";

} // namespace

namespace lq::config {

fs::path defaultConfigPath();

Configuration::Configuration(fs::path logseqPath, std::string graphName)
    : logseqPath(std::move(logseqPath)), graphName(std::move(graphName)) {}

std::optional<Configuration> loadConfig() {

    std::ifstream file{defaultConfigPath()};

    if (!file.is_open()) {
        std::cerr << "Error: could not open " << defaultConfigPath().string() << "\n";
        return std::nullopt;
    }

    std::string line;
    std::string logseqPath;
    std::string graphName;

    while (std::getline(file, line)) {
        auto parts = lq::strings::splitFirstOf(line, configDelimiter);

        if (parts.second.empty())
            continue;

        auto key = lq::strings::trim(parts.first);
        auto value = lq::strings::trim(parts.second);

        if (std::string_view(key) == configLogseqPath) {
            logseqPath = std::move(value);
        } else if (std::string_view(key) == configGraphName) {
            graphName = std::move(value);
        }
    }

    if (logseqPath.empty() || graphName.empty()) {
        std::cerr << "Error: missing required keys in " << defaultConfigPath().string() << "\n";
        return std::nullopt;
    }

    return Configuration(fs::path{logseqPath}, graphName);
}

fs::path defaultConfigPath() {
#ifdef _WIN32
    const char *home = std::getenv("USERPROFILE");
#else
    const char *home = std::getenv("HOME");
#endif
    if (!home) {
        // fallback: current dir
        return fs::current_path() / configFileName;
    }
    return fs::path(home) / configFileName;
}
} // namespace lq::config