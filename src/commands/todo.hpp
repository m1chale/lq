#pragma once

#include <filesystem>
#include <string>
#include <vector>

int runCommandTodo(const std::filesystem::path &graphPath, const std::vector<std::string> &args);
int runCommandListTodos(const std::filesystem::path &graphPath, const std::vector<std::string> &args);
