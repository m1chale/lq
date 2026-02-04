
#include <filesystem>
#include <unordered_set>
#include <vector>

namespace fs = std::filesystem;

namespace lq {

enum class SiteType { Page, Journal };
enum class TodoState { TODO, DOING, DONE, LATER, WAITING, CANCELED, UNKNOWN };

inline std::string to_string(TodoState s) {
    switch (s) {
    case TodoState::TODO:
        return "TODO";
    case TodoState::DOING:
        return "DOING";
    case TodoState::DONE:
        return "DONE";
    case TodoState::LATER:
        return "LATER";
    case TodoState::WAITING:
        return "WAITING";
    case TodoState::CANCELED:
        return "CANCELED";
    default:
        return "UNKNOWN";
    }
}

struct Site {
    std::string name;
    SiteType type;

    Site(std::string name, SiteType type);
};

struct SiteWithLines {
    Site site;
    std::vector<std::string> lines;

    SiteWithLines(Site site, std::vector<std::string> lines);
};

std::vector<std::string> getPages(const fs::path &graphPath);
std::vector<std::string> getJournals(const fs::path &graphPath);
std::vector<Site> getAllSites(const fs::path &graphPath);
std::vector<SiteWithLines> getAllLinesFromGraph(const fs::path &graphPath);
std::string_view getSiteDirectoryFromType(SiteType type);
std::optional<TodoState> getTodoStateFromLine(const std::string &line, const std::unordered_set<TodoState> &allowedStates);
TodoState markerToTodoState(const std::string &marker);
} // namespace lq