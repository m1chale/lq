#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../constants/constants.hpp"
#include "../utils/strings.hpp"

namespace fs = std::filesystem;

int runCommandPrint(const fs::path &graphPath, const std::string &siteName) {

    std::string actualSiteName = siteName;

    // add file extension if not provided
    if (!lq::strings::endsWith(siteName, std::string(lq::consts::markdownExtension)))
        actualSiteName += lq::consts::markdownExtension;

    // try pagesDirectory first, if nothing found try journals
    fs::path sitePath = graphPath / lq::consts::pagesDirectory / actualSiteName;

    std::ifstream file(sitePath);

    if (!file.is_open()) {
        sitePath = graphPath / lq::consts::journalsDirectory / actualSiteName;
        file.open(sitePath);

        if (!file.is_open()) {
            std::cerr << "Error: could not open " << sitePath << "\n";
            return 1;
        }
    }

    std::string line;
    while (std::getline(file, line)) {
        std::cout << line << "\n";
    }

    return 0;
}