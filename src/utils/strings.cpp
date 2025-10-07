#include <locale>
#include <string>
#include <string_view>
#include <vector>

namespace lq::strings {

void sortLexicographical(std::vector<std::string> &pages) {
    std::locale loc;
    std::sort(pages.begin(), pages.end(), [&loc](const std::string &a, const std::string &b) {
        return std::use_facet<std::collate<char>>(loc).compare(a.data(), a.data() + a.size(), b.data(), b.data() + b.size()) < 0;
    });
}

bool caseInsensitiveSearch(const std::string &haystack, const std::string &needle) {
    auto it = std::search(haystack.begin(), haystack.end(), needle.begin(), needle.end(),
                          [](char ch1, char ch2) { return std::tolower((unsigned char)ch1) == std::tolower((unsigned char)ch2); });
    return it != haystack.end();
};

bool endsWith(const std::string &s, const std::string &preFix) {
    if (s.size() < preFix.size())
        return false;

    for (int i = 1; i <= preFix.size(); i++) {
        if (s.at(s.size() - i) != preFix.at(preFix.size() - i))
            return false;
    }
    return true;
}

std::vector<std::string> splitFirstOf(const std::string &input, std::string_view delimiter) {
    auto position = input.find(delimiter);

    if (position == std::string::npos)
        return {input};

    std::string first = input.substr(0, position);
    std::string second = input.substr(position + delimiter.size());

    return {first, second};
}

std::string trim(std::string s) {
    auto isspaceu = [](unsigned char c) { return std::isspace(c); };
    s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspaceu));
    s.erase(std::find_if_not(s.rbegin(), s.rend(), isspaceu).base(), s.end());
    return s;
}

} // namespace lq::strings