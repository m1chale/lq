#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../constants/constants.hpp"
#include "../utils/logseq.hpp"
#include "../utils/strings.hpp"
#include "../utils/terminal.hpp"

namespace fs = std::filesystem;

inline std::string highlight_all_ci(const std::string &haystack, const std::string &needle, std::string_view color_start = "\x1b[33m",
                                    std::string_view color_reset = "\x1b[0m");

int runCommandGrep(const fs::path &graphPath, const std::string &searchString) {
    auto sites = lq::getAllSites(graphPath);

    for (auto site : sites) {
        fs::path pagePath = graphPath / lq::getSiteDirectoryFromType(site.type) / site.name;
        std::ifstream file(pagePath);
        bool printedPage = false;

        if (!file) {
            std::cerr << "Error: could not open " << pagePath << "\n";
            continue;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (lq::strings::caseInsensitiveSearch(line, searchString)) {

                if (!printedPage) {
                    printedPage = true;
                    std::cout << lq::term::green << site.name << lq::term::reset << "\n";
                }

                std::cout << highlight_all_ci(line, searchString) << "\n";
            }
        }

        if (printedPage) {
            std::cout << "\n";
        }
    }

    return 0;
}

std::string highlight_all_ci(const std::string &haystack, const std::string &needle, std::string_view color_start,
                             std::string_view color_reset) {
    if (needle.empty())
        return haystack;

    std::string out;
    out.reserve(haystack.size() + color_start.size() + color_reset.size());

    auto it = haystack.begin();
    auto end = haystack.end();

    auto eq_ci = [](char a, char b) { return std::tolower(static_cast<unsigned char>(a)) == std::tolower(static_cast<unsigned char>(b)); };

    while (it != end) {
        auto m = std::search(it, end, needle.begin(), needle.end(), eq_ci);

        if (m == end) {
            // no more matches: append tail
            out.append(it, end);
            break;
        }
        // append the text before the match
        out.append(it, m);
        // append highlighted match (using original casing)
        out.append(color_start);
        out.append(m, m + needle.size());
        out.append(color_reset);
        // continue after the match (non-overlapping)
        it = m + needle.size();
    }
    return out;
}