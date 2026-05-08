#include "OrderView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void OrderView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "        주문 관리           \n";
    std::cout << "============================\n";
    std::cout << "  1. 주문 등록\n";
    std::cout << "  2. 주문 목록 조회\n";
    std::cout << "  3. 주문 수정\n";
    std::cout << "  4. 주문 삭제\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int OrderView::getMenuChoice() const
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

void OrderView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

std::string OrderView::promptProductName() const
{
    std::string name;
    while (true)
    {
        std::cout << "제품명 입력: ";
        std::getline(std::cin, name);
        if (!name.empty()) return name;
        std::cout << "제품명은 비워둘 수 없습니다.\n";
    }
}

int OrderView::promptQuantity() const
{
    int qty;
    while (true)
    {
        std::cout << "수량 입력: ";
        if (std::cin >> qty && qty > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return qty;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "1 이상의 수량을 입력하세요.\n";
    }
}

std::string OrderView::promptStatus() const
{
    static const std::string statuses[] = { "접수", "처리 중", "완료", "취소" };

    std::cout << "  1. 접수\n"
              << "  2. 처리 중\n"
              << "  3. 완료\n"
              << "  4. 취소\n";

    int choice;
    while (true)
    {
        std::cout << "상태 선택: ";
        if (std::cin >> choice && choice >= 1 && choice <= 4)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return statuses[choice - 1];
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "1~4 사이의 값을 입력하세요.\n";
    }
}

int OrderView::promptId() const
{
    int id;
    while (true)
    {
        std::cout << "ID 입력: ";
        if (std::cin >> id && id > 0)
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return id;
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "유효한 ID를 입력하세요.\n";
    }
}

void OrderView::showOrders(const std::vector<Order>& orders) const
{
    std::cout << "\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(20) << "제품명"
              << std::setw(8)  << "수량"
              << "상태" << "\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& o : orders)
    {
        std::cout << std::setw(6)  << o.id
                  << std::setw(20) << o.productName
                  << std::setw(8)  << o.quantity
                  << o.status << "\n";
    }
    std::cout << "\n";
}

void OrderView::showOrder(const Order& order) const
{
    std::cout << "\n현재 정보"
              << "\n  ID    : " << order.id
              << "\n  제품명: " << order.productName
              << "\n  수량  : " << order.quantity
              << "\n  상태  : " << order.status << "\n\n";
}
