#include "logseq.hpp"
#include "../constants/constants.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

namespace lq {

Site::Site(std::string name, SiteType type) : name{std::move(name)}, type{type} {}
SiteWithLines::SiteWithLines(Site site, std::vector<std::string> lines) : site{std::move(site)}, lines{std::move(lines)} {}

std::vector<std::string> getViewableSitesFromPath(const fs::path &path);
bool isViewableSite(const fs::directory_entry &entry);

std::vector<std::string> getPages(const fs::path &graphPath) {
    fs::path pagesPath = graphPath / lq::consts::pagesDirectory;
    return getViewableSitesFromPath(pagesPath);
}

std::vector<std::string> getJournals(const fs::path &graphPath) {
    fs::path journalsPath = graphPath / lq::consts::journalsDirectory;
    return getViewableSitesFromPath(journalsPath);
}

std::vector<Site> getAllSites(const fs::path &graphPath) {
    auto pages = getPages(graphPath);
    auto journals = getJournals(graphPath);

    std::vector<Site> sites;
    sites.reserve(pages.size() + journals.size());

    for (auto &page : pages) {
        sites.emplace_back(std::move(page), SiteType::Page);
    }

    for (auto &journal : journals) {
        sites.emplace_back(std::move(journal), SiteType::Journal);
    }

    return sites;
}

std::vector<SiteWithLines> getAllLinesFromGraph(const fs::path &graphPath) {
    std::vector<Site> sites = getAllSites(graphPath);

    std::vector<SiteWithLines> retSites;
    retSites.reserve(sites.size());

    for (Site &site : sites) {
        fs::path pagePath = graphPath / lq::getSiteDirectoryFromType(site.type) / site.name;
        std::ifstream file(pagePath);
        std::vector<std::string> lines;

        if (!file) {
            std::cerr << "Error: could not open " << pagePath << "\n";
            continue;
        }

        std::string line;

        while (std::getline(file, line)) {
            lines.push_back(std::move(line));
        }

        retSites.emplace_back(std::move(site), std::move(lines));
    }

    return retSites;
}

std::vector<std::string> getViewableSitesFromPath(const fs::path &path) {
    std::vector<std::string> sites;

    for (const auto &entry : fs::directory_iterator(path)) {
        if (isViewableSite(entry)) {
            sites.push_back(entry.path().filename().string());
        }
    }

    return sites;
}

bool isViewableSite(const fs::directory_entry &entry) {
    return entry.is_regular_file() && entry.path().extension() == lq::consts::markdownExtension &&
           entry.path().filename().string().rfind(lq::consts::ignorePagePrefix, 0) != 0;
}

std::string_view getSiteDirectoryFromType(SiteType type) {
    switch (type) {
    case SiteType::Journal:
        return lq::consts::journalsDirectory;
    case SiteType::Page:
        return lq::consts::pagesDirectory;
    default:
        return "";
    }
}

TodoState markerToTodoState(const std::string &marker) {
    if (marker == "TODO")
        return TodoState::TODO;
    if (marker == "DOING")
        return TodoState::DOING;
    if (marker == "DONE")
        return TodoState::DONE;
    if (marker == "LATER")
        return TodoState::LATER;
    if (marker == "WAITING")
        return TodoState::WAITING;
    if (marker == "CANCELED")
        return TodoState::CANCELED;
    return TodoState::UNKNOWN;
}

std::optional<TodoState> extractTodoState(const std::string &line) {
    // Matches (optionally) a list bullet, then (optionally) a Markdown heading '#'
    // then either a checkbox [ ], [x], [-] OR one of the TODO words.
    // Also handles bare org-style "* TODO ..." headings.
    static const std::regex todoRegex(
        R"(^(?:\s*(?:[-*+]\s*)?(?:#{1,6}\s*)?|\s*\*+\s+)\s*(?:\[( |x|X|-)\]|(TODO|DOING|DONE|LATER|WAITING|CANCELED))\b)",
        std::regex::ECMAScript | std::regex::icase);

    std::smatch m;
    if (std::regex_search(line, m, todoRegex)) {
        // Checkbox match?
        if (m[1].matched) {
            char c = m[1].str()[0];
            switch (c) {
            case 'x':
            case 'X':
                return TodoState::DONE;
            case '-':
                return TodoState::DOING;
            default:
                return TodoState::TODO; // space => unchecked
            }
        }
        // Explicit marker (group 2)
        if (m[2].matched)
            return markerToTodoState(m[2].str());
    }
    return std::nullopt;
}

std::optional<TodoState> getTodoStateFromLine(const std::string &line, const std::unordered_set<TodoState> &allowedStates) {
    auto state = extractTodoState(line);
    if (!state)
        return std::nullopt;

    if (allowedStates.count(*state) > 0) {
        return state;
    }

    return std::nullopt;
}

} // namespace lq