#include "MainMenuView.h"
#include <iostream>
#include <limits>

void MainMenuView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "    ConsoleMVC v1.0         \n";
    std::cout << "============================\n";
    std::cout << "  1. 샘플 관리\n";
    std::cout << "  2. 주문 관리\n";
    std::cout << "  3. 생산 라인 관리\n";
    std::cout << "  4. 모니터링\n";
    std::cout << "  5. 출하 관리\n";
    std::cout << "  0. 종료\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int MainMenuView::getMenuChoice() const
{
    int choice;
    if (!(std::cin >> choice))
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return -1;
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    return choice;
}
