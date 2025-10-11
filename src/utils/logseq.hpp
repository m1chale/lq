
#include <filesystem>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

namespace lq {

enum class SiteType { Page, Journal };
enum class TodoState { TODO, DOING, DONE, LATER, WAITING, CANCELED, UNKNOWN };

struct Site {
    std::string name;
    SiteType type;

    Site(std::string name, SiteType type);
};

std::vector<std::string> getPages(const fs::path &graphPath);
std::vector<std::string> getJournals(const fs::path &graphPath);
std::vector<Site> getAllSites(const fs::path &graphPath);
std::string_view getSiteDirectoryFromType(SiteType type);
bool isTodoLine(const std::string &line, const std::unordered_set<TodoState> &allowedStates);
} // namespace lq