#pragma once

#include <filesystem>

namespace fs = std::filesystem;

namespace lq::config {

struct Configuration {
  public:
    fs::path logseqPath;
    std::string graphName;

    Configuration(fs::path logseqPath, std::string graphName);
};

std::optional<Configuration> loadConfig();
} // namespace lq::config