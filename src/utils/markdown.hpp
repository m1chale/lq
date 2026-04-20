#pragma once
#include <string>

class MarkdownFormatter {
  public:
    static void formatLine(std::string &line);
    static std::string colorizeTagsInLine(const std::string &line);
};