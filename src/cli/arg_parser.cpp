#include "arg_parser.hpp"
#include "../utils/strings.hpp"
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

ArgParseResult parseArgs(int argc, char *argv[], int startIndex) {
    ArgParseResult res;
    bool stopOptions = false;

    for (int i = startIndex; i < argc; ++i) {
        std::string token = argv[i];

        if (!stopOptions && token == "--") {
            stopOptions = true;
            continue;
        }

        // Long options: --key, --key=value, --key value
        if (!stopOptions && lq::strings::startsWith(token, "--")) {
            if (token.size() == 2) { // just "--"
                res.errors.push_back("Invalid option '--'");
                continue;
            }

            auto eqPos = token.find('=');
            if (eqPos != std::string::npos) {
                std::string key = token.substr(2, eqPos - 2);
                std::string val = token.substr(eqPos + 1);
                if (key.empty()) {
                    res.errors.push_back("Invalid option format: " + token);
                    continue;
                }
                res.options[key] = val;
                continue;
            }

            std::string key = token.substr(2);
            if (key.empty()) {
                res.errors.push_back("Invalid option format: " + token);
                continue;
            }

            // value only if next token doesn't look like an option
            if (i + 1 < argc) {
                std::string next = argv[i + 1];
                if (!lq::strings::startsWith(next, "-")) {
                    res.options[key] = next;
                    ++i;
                    continue;
                }
            }

            res.options[key] = std::nullopt;
            continue;
        }

        // Short options: -a or -abc
        if (!stopOptions && token.size() >= 2 && token[0] == '-' && token[1] != '-') {
            // Example: -a, -abc
            for (size_t j = 1; j < token.size(); ++j) {
                char c = token[j];
                if (c == '-') {
                    res.errors.push_back("Invalid short option in: " + token);
                    break;
                }
                std::string key(1, c);           // store as "a"
                res.options[key] = std::nullopt; // short flags are boolean here
            }
            continue;
        }

        // Positional
        res.positionals.push_back(token);
    }

    return res;
}