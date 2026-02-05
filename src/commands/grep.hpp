#include "../cli/arg_parser.hpp"
#include <expected>
#include <filesystem>
#include <string>

struct GrepOptions {
    std::string query;
    bool all = false; // -a, --all => also show the subtree below a matching line
};

std::expected<GrepOptions, std::string> parseGrepOptions(const ArgParseResult &args);

int runCommandGrep(const std::string &graphPath, const GrepOptions &opts);