#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <regex>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "../utils/logseq.hpp"
#include "../utils/markdown.hpp"
#include "../utils/strings.hpp"
#include "../utils/terminal.hpp"
#include "help.hpp"
#include "todo.hpp"

enum class TodoSubCommand { Add, List, SetDone, Unknown };

const std::unordered_map<std::string, TodoSubCommand> todoSubCommandMap = {
    {"add", TodoSubCommand::Add}, {"list", TodoSubCommand::List}, {"done", TodoSubCommand::SetDone}};

enum class TodoParametersForShowCmd { Tag };

struct TodoListParameters {
    std::vector<std::string> tags;
};

const std::unordered_map<std::string, TodoParametersForShowCmd> showSubCmdParameters = {{"-t", TodoParametersForShowCmd::Tag}};

TodoSubCommand parseSubCommand(const std::string &cmd);
std::optional<TodoListParameters> parseParametersSubCmdList(const std::vector<std::string> &args);
int runSubCommandList(const std::filesystem::path &graphPath, const TodoListParameters &parameters);
std::map<lq::TodoState, std::vector<std::string>> collectTodoLinesByState(const std::vector<lq::SiteWithLines> &sites,
                                                                          const std::unordered_set<lq::TodoState> &allowedStates);

void printTodoHeader(lq::TodoState todoState);
void removeTodoStateFromLine(std::string &line);
std::set<std::string> extractTagsFromLine(const std::string &line);
std::set<std::string> normalizeTags(const std::vector<std::string> &tags);
bool hasAnyMatchingTag(const std::set<std::string> &lineTags, const std::set<std::string> &filterTags);

int runCommandTodo(const std::filesystem::path &graphPath, const std::vector<std::string> &args) {

    if (args.empty()) {
        runCommandHelp();
        return 0;
    }

    std::string rawCmd = args[0];

    TodoSubCommand cmd = parseSubCommand(rawCmd);

    switch (cmd) {
    case TodoSubCommand::Add: {
        break;
    }
    case TodoSubCommand::List: {
        const std::vector<std::string> listArgs(args.begin() + 1, args.end());
        return runCommandListTodos(graphPath, listArgs);
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

int runCommandListTodos(const std::filesystem::path &graphPath, const std::vector<std::string> &args) {
    auto paras = parseParametersSubCmdList(args);

    if (!paras)
        return -1;

    return runSubCommandList(graphPath, *paras);
}

TodoSubCommand parseSubCommand(const std::string &cmd) {
    auto it = todoSubCommandMap.find(cmd);

    if (it == todoSubCommandMap.end()) {
        return TodoSubCommand::Unknown;
    }

    return it->second;
}

std::optional<TodoListParameters> parseParametersSubCmdList(const std::vector<std::string> &args) {

    TodoListParameters parameters;

    for (size_t i = 0; i < args.size(); i++) {
        auto it = showSubCmdParameters.find(args[i]);

        if (it == showSubCmdParameters.end()) {
            std::cerr << "parameter not found: " << args[i] << "\n";
            return std::nullopt;
        }

        switch (it->second) {
        case TodoParametersForShowCmd::Tag: {

            if (i + 1 >= args.size()) {
                std::cerr << "no value provided for parameter: " << args[i] << "\n";
                return std::nullopt;
            }

            parameters.tags.push_back(args[i + 1]);
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
    const std::set<std::string> filterTags = normalizeTags(parameters.tags);

    std::map<lq::TodoState, std::vector<std::string>> todos = collectTodoLinesByState(sites, allowedStates);

    for (const auto &todo : todos) {
        std::map<std::string, std::vector<std::string>> groupedTodos;

        for (std::string line : todo.second) {
            removeTodoStateFromLine(line);

            std::set<std::string> lineTags = extractTagsFromLine(line);

            if (!filterTags.empty() && !hasAnyMatchingTag(lineTags, filterTags))
                continue;

            if (lineTags.empty()) {
                groupedTodos["#untagged"].push_back(line);
            } else {
                // Keep a todo line in each of its tags so grouped browsing stays useful.
                for (const std::string &tag : lineTags)
                    groupedTodos[tag].push_back(line);
            }
        }

        if (groupedTodos.empty())
            continue;

        printTodoHeader(todo.first);

        for (const auto &[tag, lines] : groupedTodos) {
            std::cout << "  " << lq::term::bold_on << lq::term::yellow << tag << lq::term::resetHard << "\n";

            for (const std::string &line : lines) {
                std::cout << "\t" << MarkdownFormatter::colorizeTagsInLine(line) << "\n";
            }
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
    std::cout << lq::term::bold_on << lq::term::blue << "[" << lq::to_string(todoState) << "]" << lq::term::resetHard << "\n";
}

void removeTodoStateFromLine(std::string &line) {
    constexpr std::string_view delimiter = " ";
    auto parts = lq::strings::split(line, delimiter);

    parts.erase(std::remove_if(parts.begin(), parts.end(),
                               [](const std::string &tok) { return lq::markerToTodoState(tok) != lq::TodoState::UNKNOWN; }),
                parts.end());

    line = lq::strings::join(parts, delimiter);
}

std::set<std::string> extractTagsFromLine(const std::string &line) {
    static const std::regex tagPattern(R"((^|[^A-Za-z0-9_])(#[A-Za-z0-9_-]+))");

    std::set<std::string> tags;
    auto begin = std::sregex_iterator(line.begin(), line.end(), tagPattern);
    auto end = std::sregex_iterator();

    for (auto it = begin; it != end; ++it) {
        tags.insert((*it).str(2));
    }

    return tags;
}

std::set<std::string> normalizeTags(const std::vector<std::string> &tags) {
    std::set<std::string> normalized;

    for (const std::string &tag : tags) {
        if (tag.empty())
            continue;

        if (tag.front() == '#')
            normalized.insert(tag);
        else
            normalized.insert("#" + tag);
    }

    return normalized;
}

bool hasAnyMatchingTag(const std::set<std::string> &lineTags, const std::set<std::string> &filterTags) {
    for (const std::string &tag : lineTags) {
        if (filterTags.contains(tag))
            return true;
    }

    return false;
}