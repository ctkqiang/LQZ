#include "view.h"

#include <algorithm>
#include <iostream>

const std::string View::RED = "\033[31m";
const std::string View::BLUE = "\033[34m";
const std::string View::GREEN = "\033[32m";
const std::string View::YELLOW = "\033[33m";
const std::string View::WHITE = "\033[0m";

void View::DisplayHeader() {
    std::cout << BLUE << "\033[36m" << R"(
       _______________
      /               \
     /                 \
    |  _______________  |
    | |               | |
    | |   \033[37m████████\033[36m    | |
    | |   \033[37m█  \033[36mPHONE \033[37m█\033[36m  | |
    | |   \033[37m████████\033[36m    | |
    | |               | |
    | |_______________| |
    |        [ ]        |
     \                 /
      \_______________/
)" << "\033[0m"
              << WHITE << std::endl;
}

void View::ClearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

std::string View::GetOSType() {
#ifdef _WIN32
    return "Windows";
#elif __APPLE__
    return "MacOS";
#elif __linux__
    return "Linux";
#elif __unix__
    return "Unix";
#else
    return "Unknown OS";
#endif
}
