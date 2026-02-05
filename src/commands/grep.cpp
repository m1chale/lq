#include "grep.hpp"
#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../cli/arg_parser.hpp"
#include "../constants/constants.hpp"
#include "../utils/logseq.hpp"
#include "../utils/strings.hpp"
#include "../utils/terminal.hpp"

namespace fs = std::filesystem;

static constexpr size_t kTabSize = 4; // treats tabs as 4 spaces

std::expected<GrepOptions, std::string> parseGrepOptions(const ArgParseResult &args) {
    GrepOptions o;

    // required positional: the search string/pattern
    if (args.positionals.empty()) {
        return std::unexpected("grep: missing required <pattern>");
    }
    o.query = args.positionals[0];

    // -a or --all
    o.all = args.options.contains("a") || args.options.contains("all");

    // optionally reject extra args
    if (args.positionals.size() > 1) {
        return std::unexpected("grep: too many arguments (unexpected: " + args.positionals[1] + ")");
    }

    return o;
}

inline std::string highlight_all_ci(const std::string &haystack, const std::string &needle, std::string_view color_start = "\x1b[33m",
                                    std::string_view color_reset = "\x1b[0m");

// counts leading spaces and tabs
static size_t calculateIndentationWidth(const std::string &s) {
    size_t n = 0;
    for (char ch : s) {
        if (ch == ' ')
            n += 1;
        else if (ch == '\t')
            n += kTabSize;
        else
            break;
    }
    return n;
}

static size_t leadingWhitespaceCharacterCount(const std::string &s, size_t widthToRemove) {
    size_t width = 0;
    size_t chars = 0;

    for (char ch : s) {
        if (ch == ' ') {
            width += 1;
            chars += 1;
        } else if (ch == '\t') {
            width += kTabSize;
            chars += 1;
        } else
            break;

        if (width >= widthToRemove)
            break;
    }
    return chars;
}

static std::string_view dedentView(const std::string &s, size_t widthToRemove) {
    const size_t cut = leadingWhitespaceCharacterCount(s, widthToRemove);
    return std::string_view(s).substr(cut);
}

int runCommandGrep(const std::string &graphPath, const GrepOptions &opts) {
    std::vector<lq::SiteWithLines> sites = lq::getAllLinesFromGraph(graphPath);

    for (const lq::SiteWithLines &siteWithLines : sites) {
        bool printedPage = false;

        const auto &lines = siteWithLines.lines;
        for (size_t i = 0; i < lines.size(); ++i) {
            const std::string &line = lines[i];

            if (!lq::strings::caseInsensitiveSearch(line, opts.query)) {
                continue;
            }

            if (!printedPage) {
                printedPage = true;
                std::cout << lq::term::green << siteWithLines.site.name << lq::term::reset << "\n";
            }

            const size_t baseIndent = calculateIndentationWidth(line);

            // print the matching line
            std::string dedented = std::string(dedentView(line, baseIndent));
            std::cout << highlight_all_ci(dedented, opts.query) << "\n";

            if (opts.all) {
                // print children (subtree) based on indentation

                size_t j = i + 1;
                while (j < lines.size()) {
                    const std::string &next = lines[j];

                    // blank lines: decide whether they belong to subtree
                    // In markdown files, blank lines usually still "belong"; we keep them if indent > baseIndent OR line empty
                    if (next.empty()) {
                        std::cout << next << "\n";
                        ++j;
                        continue;
                    }

                    const size_t nextIndent = calculateIndentationWidth(next);
                    if (nextIndent <= baseIndent)
                        break;

                    std::string dedented = std::string(dedentView(next, baseIndent));
                    std::cout << highlight_all_ci(dedented, opts.query) << "\n";
                    ++j;
                }

                // skip over the already printed subtree
                i = (j == 0) ? i : (j - 1);
            }
        }

        if (printedPage)
            std::cout << "\n";
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