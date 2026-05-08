#include "ProductionLineView.h"
#include <iostream>
#include <limits>

void ProductionLineView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "      생산 라인 관리        \n";
    std::cout << "============================\n";
    std::cout << "  1. 라인 등록\n";
    std::cout << "  2. 라인 목록 조회\n";
    std::cout << "  3. 라인 상태 수정\n";
    std::cout << "  4. 라인 삭제\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int ProductionLineView::getMenuChoice() const
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

void ProductionLineView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}
