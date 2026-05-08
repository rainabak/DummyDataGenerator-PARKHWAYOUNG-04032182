#include "ConsoleUtil.h"
#include <iostream>
#include <limits>
#include <windows.h>

void ConsoleUtil::clearScreen()
{
    system("cls");
}

void ConsoleUtil::printSeparator()
{
    std::cout << "----------------------------\n";
}

void ConsoleUtil::pause()
{
    std::cout << "\n계속하려면 Enter를 누르세요...";
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}
