#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <string>

#include "../utils/logseq.hpp"
#include "../utils/terminal.hpp"
#include "help.hpp"
#include <unordered_set>

enum class TodoSubCommand { Add, List, SetDone, Unknown };

const std::unordered_map<std::string, TodoSubCommand> todoSubCommandMap = {
    {"add", TodoSubCommand::Add}, {"list", TodoSubCommand::List}, {"done", TodoSubCommand::SetDone}};

enum class TodoParametersForShowCmd { Tag };

struct TodoListParameters {
    std::vector<std::string> tags;
};

const std::unordered_map<std::string, TodoParametersForShowCmd> showSubCmdParameters = {{"-t", TodoParametersForShowCmd::Tag}};

TodoSubCommand parseSubCommand(const std::string &cmd);

std::optional<TodoListParameters> parseParametersSubCmdList(int argc, char *argv[]);
int runSubCommandList(const std::filesystem::path &graphPath, const TodoListParameters &parameters);

int runCommandTodo(const std::filesystem::path &graphPath, int argc, char *argv[]) {

    if (argc < 3) {
        runCommandHelp();
        return 0;
    }

    std::string rawCmd = argv[2];

    TodoSubCommand cmd = parseSubCommand(rawCmd);

    switch (cmd) {
    case TodoSubCommand::Add: {
        break;
    }
    case TodoSubCommand::List: {
        auto paras = parseParametersSubCmdList(argc, argv);

        if (!paras)
            return -1;

        return runSubCommandList(graphPath, *paras);
    }
    case TodoSubCommand::SetDone: {
        break;
    }
    case TodoSubCommand::Unknown: {
        std::cerr << "command not found: " << rawCmd << "\n";
        return -1;
    }
    }

    return 0;
}

TodoSubCommand parseSubCommand(const std::string &cmd) {
    auto it = todoSubCommandMap.find(cmd);

    if (it == todoSubCommandMap.end()) {
        return TodoSubCommand::Unknown;
    }

    return it->second;
}

std::optional<TodoListParameters> parseParametersSubCmdList(int argc, char *argv[]) {

    TodoListParameters parameters;

    if (argc < 4)
        return parameters;

    for (int i = 3; i < argc; i++) {
        auto it = showSubCmdParameters.find(argv[i]);

        if (it == showSubCmdParameters.end()) {
            std::cerr << "parameter not found: " << argv[i] << "\n";
            return std::nullopt;
        }

        switch (it->second) {
        case TodoParametersForShowCmd::Tag: {

            if (i + 1 >= argc) {
                std::cerr << "no value provided for parameter: " << argv[i] << "\n";
                return std::nullopt;
            }

            parameters.tags.push_back(argv[i + 1]);
            i++;
            break;
        }
        }
    }

    return parameters;
}

int runSubCommandList(const std::filesystem::path &graphPath, const TodoListParameters &parameters) {

    std::vector<lq::Site> sites = lq::getAllSites(graphPath);

    // TODO: same loop exists in grep function - move to own function
    for (auto site : sites) {
        fs::path pagePath = graphPath / lq::getSiteDirectoryFromType(site.type) / site.name;
        std::ifstream file(pagePath);

        if (!file) {
            std::cerr << "Error: could not open " << pagePath << "\n";
            continue;
        }

        std::string line;
        std::unordered_set<lq::TodoState> allowedStates = {lq::TodoState::TODO, lq::TodoState::DOING, lq::TodoState::LATER,
                                                           lq::TodoState::WAITING};

        while (std::getline(file, line)) {
            if (lq::isTodoLine(line, allowedStates)) {
                std::cout << line << "\n";
            }
        }
    }

    return 0;
}