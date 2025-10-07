
#include <filesystem>
#include <vector>

namespace fs = std::filesystem;

namespace lq {

enum class SiteType { Page, Journal };

struct Site {
    std::string name;
    SiteType type;

    Site(std::string name, SiteType type);
};

std::vector<std::string> getPages(const fs::path &graphPath);
std::vector<std::string> getJournals(const fs::path &graphPath);
std::vector<Site> getAllSites(const fs::path &graphPath);
std::string_view getSiteDirectoryFromType(SiteType type);
} // namespace lq