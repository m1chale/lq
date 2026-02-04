#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <string>
#include <unordered_map>

#include "../utils/logseq.hpp"
#include "../utils/strings.hpp"
#include "../utils/terminal.hpp"
#include "help.hpp"
#include <map>
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
std::map<lq::TodoState, std::vector<std::string>> collectTodoLinesByState(const std::vector<lq::SiteWithLines> &sites,
                                                                          const std::unordered_set<lq::TodoState> &allowedStates);

void printTodoHeader(lq::TodoState todoState);
void removeTodoStateFromLine(std::string &line);

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
    std::vector<lq::SiteWithLines> sites = lq::getAllLinesFromGraph(graphPath);

    const std::unordered_set<lq::TodoState> allowedStates = {lq::TodoState::TODO, lq::TodoState::DOING, lq::TodoState::LATER,
                                                             lq::TodoState::WAITING};

    std::map<lq::TodoState, std::vector<std::string>> todos = collectTodoLinesByState(sites, allowedStates);

    for (std::__1::pair<const lq::TodoState, std::__1::vector<std::__1::string>> todo : todos) {
        printTodoHeader(todo.first);

        for (std::string line : todo.second) {
            removeTodoStateFromLine(line);
            std::cout << line << "\n";
        }

        std::cout << std::endl;
    }
    return 0;
}

std::map<lq::TodoState, std::vector<std::string>> collectTodoLinesByState(const std::vector<lq::SiteWithLines> &sites,
                                                                          const std::unordered_set<lq::TodoState> &allowedStates) {
    std::map<lq::TodoState, std::vector<std::string>> todos;

    for (lq::SiteWithLines site : sites) {
        for (std::string line : site.lines) {
            auto state = lq::getTodoStateFromLine(line, allowedStates);
            if (state) {
                todos[*state].push_back(lq::strings::trim(line));
            }
        }
    }

    return todos;
}

void printTodoHeader(lq::TodoState todoState) {
    std::cout << lq::term::bold << lq::term::blue << "[" << lq::to_string(todoState) << "]" << lq::term::reset << "\n";
}

void removeTodoStateFromLine(std::string &line) {
    constexpr std::string_view delimiter = " ";
    auto parts = lq::strings::split(line, delimiter);

    parts.erase(std::remove_if(parts.begin(), parts.end(),
                               [](const std::string &tok) { return lq::markerToTodoState(tok) != lq::TodoState::UNKNOWN; }),
                parts.end());

    line = lq::strings::join(parts, delimiter);
}