#include "ConsoleView.h"
#include <iostream>
#include <limits>
#include <string>

// ── 메뉴 출력 ─────────────────────────────────────────────────────────────────

void ConsoleView::showMainMenu() const
{
    std::cout << "\n========================================\n";
    std::cout << "       DummyDataGenerator\n";
    std::cout << "========================================\n";
    std::cout << " 1. DataMonitor (기존 시스템)\n";
    std::cout << " 2. Dummy 데이터 생성\n";
    std::cout << " 0. 종료\n";
    std::cout << "========================================\n";
    std::cout << " 선택 > ";
}

void ConsoleView::showGenerateMenu() const
{
    std::cout << "\n========================================\n";
    std::cout << "       Dummy 데이터 생성\n";
    std::cout << "========================================\n";
    std::cout << " 1. Sample Dummy 생성\n";
    std::cout << " 2. Order Dummy 생성\n";
    std::cout << " 3. ProductionLine Dummy 생성\n";
    std::cout << " 4. 전체 Dummy 생성\n";
    std::cout << " 0. 돌아가기\n";
    std::cout << "========================================\n";
    std::cout << " 선택 > ";
}

// ── 결과·오류 출력 ────────────────────────────────────────────────────────────

void ConsoleView::showResult(const std::string& target,
                              int                count,
                              const std::string& filePath) const
{
    std::cout << "----------------------------------------\n";
    std::cout << " [완료] " << target << " " << count << "건 생성\n";
    std::cout << "        저장 파일 : " << filePath << "\n";
    std::cout << "----------------------------------------\n";
}

void ConsoleView::showError(const std::string& message) const
{
    std::cout << "\n [오류] " << message << "\n";
}

void ConsoleView::showOverwriteWarning() const
{
    std::cout << " [주의] 기존 데이터가 모두 삭제됩니다.\n";
}

// ── 입력 ──────────────────────────────────────────────────────────────────────

int ConsoleView::readMainMenuChoice() const
{
    int choice = -1;
    while (true)
    {
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " [오류] 숫자를 입력하세요 > ";
            continue;
        }
        if (choice >= 0 && choice <= 2) break;
        std::cout << " [오류] 0~2 사이의 숫자를 입력하세요 > ";
    }
    return choice;
}

int ConsoleView::readMenuChoice() const
{
    int choice = -1;
    while (true)
    {
        if (!(std::cin >> choice))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " [오류] 숫자를 입력하세요 > ";
            continue;
        }
        if (choice >= 0 && choice <= 4) break;
        std::cout << " [오류] 0~4 사이의 숫자를 입력하세요 > ";
    }
    return choice;
}

int ConsoleView::readCount(int max, const std::string& target) const
{
    if (!target.empty())
        std::cout << " [" << target << "]";
    std::cout << " 생성 수량 입력 (1 ~ " << max << "): ";

    int count = -1;
    while (true)
    {
        if (!(std::cin >> count))
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << " [오류] 숫자를 입력하세요: ";
            continue;
        }
        if (count >= 1 && count <= max) break;
        if (count < 1)
            std::cout << " [오류] 1 이상의 수를 입력하세요: ";
        else
            std::cout << " [오류] 최대 " << max << "건까지 가능합니다: ";
    }
    return count;
}

bool ConsoleView::readOverwrite() const
{
    std::cout << " 기존 데이터 처리 (Y: 덮어쓰기 / N: 추가): ";
    std::string input;
    while (true)
    {
        std::cin >> input;
        if (input == "Y" || input == "y") return true;
        if (input == "N" || input == "n") return false;
        std::cout << " [오류] Y 또는 N을 입력하세요: ";
    }
}

void ConsoleView::pause() const
{
    std::cout << "\n 계속하려면 엔터를 누르세요...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
