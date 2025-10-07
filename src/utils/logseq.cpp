#include "logseq.hpp"
#include "../constants/constants.hpp"
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace lq {

Site::Site(std::string name, SiteType type) : name{name}, type{type} {};

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

} // namespace lq