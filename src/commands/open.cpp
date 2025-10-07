#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include "../constants/constants.hpp"

namespace fs = std::filesystem;

static std::string url_encode(const std::string &);
int open_logseq_page_sys(const std::string &graph, const std::string &pageTitle);

int runCommandOpen(const std::string &graphName, const std::string &pageName) {
    std::string pageNameWithoutExtension = pageName;

    int pageExtensionLength = lq::consts::markdownExtension.size();

    if (pageName.size() >= pageExtensionLength &&
        pageName.compare(pageName.size() - pageExtensionLength, pageExtensionLength, lq::consts::markdownExtension) == 0) {
        pageNameWithoutExtension = pageName.substr(0, pageName.size() - pageExtensionLength);
    }

    return open_logseq_page_sys(graphName, pageNameWithoutExtension);
}

static std::string url_encode(const std::string &s) {
    // Percent-encode bytes not in [A-Za-z0-9-_.~]
    std::string out;
    const char *hex = "0123456789ABCDEF";
    for (unsigned char c : s) {
        if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') {
            out += static_cast<char>(c);
        } else {
            out += '%';
            out += hex[(c >> 4) & 0xF];
            out += hex[c & 0xF];
        }
    }
    return out;
}

int open_logseq_page_sys(const std::string &graph, const std::string &pageTitle) {
    const std::string url = "logseq://graph/" + url_encode(graph) + "?page=" + url_encode(pageTitle);
    // Avoid shell injection by not interpolating untrusted input; at minimum, single-quote:
    const std::string cmd = "/usr/bin/open '" + url + "'";
    return std::system(cmd.c_str());
}