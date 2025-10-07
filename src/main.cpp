#include <iostream>

#include "commands/commander.hpp"
#include "commands/grep.hpp"
#include "commands/help.hpp"
#include "commands/list.hpp"
#include "commands/open.hpp"
#include "commands/print.hpp"
#include "config/config.hpp"
#include "utils/strings.hpp"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        runCommandHelp();
        return 0;
    }

    auto config = lq::config::loadConfig();

    if (!config) {
        return -1;
    }

    std::string cmdArgument = argv[1];

    Command cmd = parseCommand(cmdArgument);

    switch (cmd) {
    case Command::ListPages: {
        return runCommandList(config->logseqPath, true, false);
        break;
    }
    case Command::ListJournals: {
        return runCommandList(config->logseqPath, false, true);
        break;
    }
    case Command::ListSites: {
        return runCommandList(config->logseqPath, true, true);
        break;
    }
    case Command::openSite: {
        if (argc < 3) {
            std::cerr << "Not enough arguments provided";
        }

        return runCommandOpen(config->graphName, argv[2]);
        break;
    }
    case Command::PrintSite: {
        if (argc < 3) {
            std::cerr << "Not enough arguments provided";
        }

        return runCommandPrint(config->logseqPath, argv[2]);
        break;
    }
    case Command::GrepSites: {

        return runCommandGrep(config->logseqPath, argv[2]);
        break;
    }

    case Command::Set: {
        break;
    }
    case Command::Help: {
        return runCommandHelp();
        break;
    }
    case Command::Unknown: {
        std::cerr << "command not found";
        return -1;
    }
    }

    return 0;
}
