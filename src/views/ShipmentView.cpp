#include "ShipmentView.h"
#include <iostream>
#include <limits>

void ShipmentView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "        출하 관리           \n";
    std::cout << "============================\n";
    std::cout << "  1. 출하 등록\n";
    std::cout << "  2. 출하 목록 조회\n";
    std::cout << "  3. 출하 상태 수정\n";
    std::cout << "  4. 출하 삭제\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int ShipmentView::getMenuChoice() const
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

void ShipmentView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}
