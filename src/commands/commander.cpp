#include "commander.hpp"
#include <unordered_map>

std::unordered_map<std::string, Command> commandMap = {
    {"help", Command::Help},     {"pages", Command::ListPages}, {"journals", Command::ListJournals}, {"sites", Command::ListSites},
    {"open", Command::openSite}, {"cat", Command::PrintSite},   {"grep", Command::GrepSites},        {"set", Command::Set}};

Command parseCommand(const std::string cmd) {
    auto it = commandMap.find(cmd);

    if (it == commandMap.end()) {
        return Command::Unknown;
    }

    return it->second;
}
