#include "../constants/constants.hpp"
#include "../utils/logseq.hpp"
#include "../utils/strings.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int runCommandList(const fs::path &graphPath, bool listPages, bool listJournals) {

    std::vector<std::string> sites;

    if (listPages && listJournals) {
        auto sitesVec = lq::getAllSites(graphPath);

        for (auto site : sitesVec) {
            sites.push_back(site.name);
        }

    } else if (listPages) {
        sites = lq::getPages(graphPath);
    } else if (listJournals) {
        sites = lq::getJournals(graphPath);
    }

    if (sites.empty()) {
        std::cerr << "nothing found\n";
        return -1;
    }

    lq::strings::sortLexicographical(sites);

    for (const std::string &site : sites) {
        std::cout << site << "\n";
    }

    return 0;
}
