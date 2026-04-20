#include "markdown.hpp"
#include "strings.hpp"
#include "terminal.hpp"

#include <string_view>

static bool parseHeading(std::string_view line, std::size_t &prefixLen, std::string_view &title) {
    std::size_t i = 0;

    // 1) keep indentation as prefix
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t'))
        ++i;

    // 2) optional list marker "- " / "* " / "+ "
    if (i + 1 < line.size() && (line[i] == '-' || line[i] == '*' || line[i] == '+') && line[i + 1] == ' ') {
        i += 2;
    }

    // 3) count hashes
    std::size_t hashStart = i;
    std::size_t count = 0;
    while (i < line.size() && line[i] == '#') {
        ++i;
        ++count;
    }

    // must be 1..6 hashes and followed by space
    if (count == 0 || count > 6)
        return false;
    if (i >= line.size() || line[i] != ' ')
        return false;

    // prefix length: everything up to and including the space after hashes
    prefixLen = i + 1;
    title = line.substr(prefixLen);
    return true;
}

// do some very basic markdown parsing and format the line
// currently only bold text and headers are formatted
void MarkdownFormatter::formatLine(std::string &line) {
    std::string out;
    out.reserve(line.size());

    std::size_t prefixLen = 0;
    std::string_view title;

    if (parseHeading(line, prefixLen, title)) {
        // keep the prefix (indent + "- " + "### " etc.) or drop the hashes—your choice.
        // Option A: KEEP list prefix but REMOVE the hashes:
        // - We’ll rebuild: indent + "- " + styled(title)

        // Keep everything before the hashes? For simplicity: keep indentation + list marker only.
        // Here: keep indentation + optional "- " exactly as in the original line, but remove the hashes part.
        // That means: take original up to where hashes started? We didn't return that,
        // so simplest: keep original up to (prefixLen), but that includes the hashes.
        // Instead: easiest UX: keep indentation and optional "- " by re-parsing quickly:

        // Quick: compute "lead" = indentation + optional bullet
        std::size_t leadEnd = 0;
        while (leadEnd < line.size() && (line[leadEnd] == ' ' || line[leadEnd] == '\t'))
            ++leadEnd;
        if (leadEnd + 1 < line.size() && (line[leadEnd] == '-' || line[leadEnd] == '*' || line[leadEnd] == '+') && line[leadEnd + 1] == ' ')
            leadEnd += 2;

        out.append(line, 0, leadEnd);
        out += lq::term::bold_on;
        out += lq::term::underline_on;
        out += lq::term::red; // or red, or none
        out.append(title);
        out += lq::term::fg_reset; // reset only color
        out += lq::term::underline_off;
        out += lq::term::bold_off;

        line = std::move(out);
        return;
    }

    bool inBold = false;

    auto isEscaped = [&](std::size_t pos) -> bool { return pos > 0 && line[pos - 1] == '\\'; };

    auto followedBySpace = [&](std::size_t pos, std::size_t markerLen) -> bool {
        return (pos + markerLen < line.size()) && line[pos + markerLen] == ' ';
    };

    for (std::size_t i = 0; i < line.size();) {

        // Bold marker: ** ... **  or  __ ... __
        if (i + 1 < line.size()) {
            const char a = line[i];
            const char b = line[i + 1];
            const bool isBoldMarker = (a == '*' && b == '*') || (a == '_' && b == '_');

            if (isBoldMarker) {
                if (isEscaped(i)) {
                    out += a;
                    out += b;
                    i += 2;
                    continue;
                }

                if (!inBold && followedBySpace(i, 2)) {
                    out += a;
                    out += b;
                    i += 2;
                    continue;
                }

                out += inBold ? lq::term::bold_off : lq::term::bold_on;
                inBold = !inBold;
                i += 2;
                continue;
            }
        }

        out += line[i];
        ++i;
    }

    // Fallback: prevent style leaking to next line
    if (inBold)
        out += lq::term::bold_off;

    line = std::move(out);
}
