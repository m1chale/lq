#include "../utils/terminal.hpp"
#include <iostream>

int runCommandHelp() {
    std::cout << "\t************************************************************************\n";
    std::cout << "\t* lq offers the following commands:\n";
    std::cout << "\t*\n";
    std::cout << "\t* " << lq::term::bold << "lq sites" << lq::term::reset << "\t\t\t\t: for listing all the pages and journals\n";
    std::cout << "\t* " << lq::term::bold << "lq pages" << lq::term::reset << "\t\t\t\t: for listing all the pages\n";
    std::cout << "\t* " << lq::term::bold << "lq journals" << lq::term::reset << "\t\t\t\t: for listing all the journals\n";
    std::cout << "\t* " << lq::term::bold << "lq cat" << lq::term::reset << " <site-name>\t\t\t: for printing a site\n";
    std::cout << "\t* " << lq::term::bold << "lq grep" << lq::term::reset
              << " <search-string>\t\t: for searching through all the pages/journals\n";
    std::cout << "\t* " << lq::term::bold << "lq open" << lq::term::reset
              << " <site-name>\t\t\t: for opening a page/journal in logseq app\n";
    std::cout << "\t* " << lq::term::bold << "lq set" << lq::term::reset
              << " <path-name>\t\t\t: to set the path to your logseq graph TODO\n";
    std::cout << "\t* " << lq::term::bold << "lq todo list" << lq::term::reset << "\t\t\t\t: for listing all todos\n";
    std::cout << "\t*\n\t************************************************************************\n";

    return 0;
}