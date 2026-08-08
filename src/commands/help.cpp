#include "../utils/terminal.hpp"
#include <iostream>

int runCommandHelp() {
    std::cout << "\t************************************************************************\n";
    std::cout << "\t* lq offers the following commands:\n";
    std::cout << "\t*\n";
    std::cout << "\t* " << lq::term::make_bold("lq sites") << "\t\t\t\t: for listing all the pages and journals\n";
    std::cout << "\t* " << lq::term::make_bold("lq pages") << "\t\t\t\t: for listing all the pages\n";
    std::cout << "\t* " << lq::term::make_bold("lq journals") << "\t\t\t\t: for listing all the journals\n";
    std::cout << "\t* " << lq::term::make_bold("lq cat") << " <site-name>\t\t\t: for printing a site\n";
    std::cout << "\t* " << lq::term::make_bold("lq grep") << " <search-string>\t\t: for searching through all the pages/journals\n";
    std::cout << "\t* " << lq::term::make_bold("lq open") << " <site-name>\t\t\t: for opening a page/journal in logseq app\n";
    std::cout << "\t* " << lq::term::make_bold("lq set") << " <path-name>\t\t\t: to set the path to your logseq graph TODO\n";
    std::cout << "\t* " << lq::term::make_bold("lq todo list") << "\t\t\t\t: for listing all todos\n";
    std::cout << "\t* " << lq::term::make_bold("lq todos") << "\t\t\t\t: shortcut for listing all todos\n";
    std::cout << "\t*\n\t************************************************************************\n";

    return 0;
}
