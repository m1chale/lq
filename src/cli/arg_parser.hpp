#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

struct ArgParseResult {
    std::vector<std::string> positionals;
    std::unordered_map<std::string, std::optional<std::string>> options;
    std::vector<std::string> errors;
};

ArgParseResult parseArgs(int argc, char *argv[], int startIndex);