#include <string>

namespace lq::strings {

void sortLexicographical(std::vector<std::string> &pages);
bool caseInsensitiveSearch(const std::string &haystack, const std::string &needle);
bool startsWith(const std::string &s, const char *prefix);
bool endsWith(const std::string &s, const std::string &preFix);
std::vector<std::string> split(std::string input, std::string_view delimiter);
std::pair<std::string, std::string> splitFirstOf(const std::string &input, std::string_view delimiter);
std::string join(const std::vector<std::string> &input, std::string_view delimiter);
std::string trim(std::string s);
} // namespace lq::strings