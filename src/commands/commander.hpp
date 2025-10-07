#pragma once

#include <string>
enum class Command { Help, ListPages, ListJournals, ListSites, openSite, PrintSite, GrepSites, Set, Unknown };

Command parseCommand(const std::string cmd);