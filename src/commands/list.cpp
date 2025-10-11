#include "../constants/constants.hpp"
#include "../utils/logseq.hpp"
#include "../utils/strings.hpp"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

int runCommandList(const fs::path &graphPath, bool listPages, bool listJournals) {

    std::vector<std::string> siteNames;

    if (listPages && listJournals) {
        auto sitesVec = lq::getAllSites(graphPath);

        for (auto site : sitesVec) {
            siteNames.push_back(site.name);
        }

    } else if (listPages) {
        siteNames = lq::getPages(graphPath);
    } else if (listJournals) {
        siteNames = lq::getJournals(graphPath);
    }

    if (siteNames.empty()) {
        std::cerr << "nothing found\n";
        return -1;
    }

    lq::strings::sortLexicographical(siteNames);

    for (const std::string &site : siteNames) {
        std::cout << site << "\n";
    }

    return 0;
}
