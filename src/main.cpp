#include <iostream>

#include "cli/arg_parser.hpp"
#include "commands/commander.hpp"
#include "commands/grep.hpp"
#include "commands/help.hpp"
#include "commands/list.hpp"
#include "commands/open.hpp"
#include "commands/print.hpp"
#include "commands/todo.hpp"
#include "config/config.hpp"
#include "utils/strings.hpp"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        runCommandHelp();
        return EXIT_SUCCESS;
    }

    auto config = lq::config::loadConfig();

    if (!config) {
        return EXIT_FAILURE;
    }

    std::string cmdArgument = argv[1];

    Command cmd = parseCommand(cmdArgument);
    ArgParseResult parsedArguments = parseArgs(argc, argv, 2);

    if (!parsedArguments.errors.empty()) {
        for (auto &e : parsedArguments.errors)
            std::cerr << e << "\n";
        return EXIT_FAILURE;
    }

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
            return EXIT_FAILURE;
        }

        return runCommandOpen(config->graphName, argv[2]);
        break;
    }
    case Command::PrintSite: {
        if (argc < 3) {
            std::cerr << "Not enough arguments provided";
            return EXIT_FAILURE;
        }

        return runCommandPrint(config->logseqPath, argv[2]);
        break;
    }
    case Command::GrepSites: {
        auto opts = parseGrepOptions(parsedArguments);
        if (!opts) {
            std::cerr << opts.error() << "\n";
            return EXIT_FAILURE;
        }
        return runCommandGrep(config->logseqPath, *opts);
        break;
    }

    case Command::Todo: {

        return runCommandTodo(config->logseqPath, argc, argv);
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
        return EXIT_FAILURE;
    }
    }

    return EXIT_FAILURE;
}
