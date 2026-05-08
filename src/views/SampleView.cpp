#include "SampleView.h"
#include <iomanip>
#include <iostream>
#include <limits>

void SampleView::showMenu() const
{
    std::cout << "============================\n";
    std::cout << "        샘플 관리           \n";
    std::cout << "============================\n";
    std::cout << "  1. 샘플 등록\n";
    std::cout << "  2. 샘플 목록 조회\n";
    std::cout << "  3. 샘플 수정\n";
    std::cout << "  4. 샘플 삭제\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << "============================\n";
    std::cout << "선택: ";
}

int SampleView::getMenuChoice() const
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

void SampleView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

std::string SampleView::promptName() const
{
    std::string name;
    while (true)
    {
        std::cout << "이름 입력: ";
        std::getline(std::cin, name);
        if (!name.empty()) return name;
        std::cout << "이름은 비워둘 수 없습니다.\n";
    }
}

std::string SampleView::promptDescription() const
{
    std::string desc;
    std::cout << "설명 입력 (생략 가능): ";
    std::getline(std::cin, desc);
    return desc;
}

int SampleView::promptId() const
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

void SampleView::showSamples(const std::vector<Sample>& samples) const
{
    std::cout << "\n";
    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(20) << "이름"
              << "설명" << "\n";
    std::cout << std::string(50, '-') << "\n";
    for (const auto& s : samples)
    {
        std::cout << std::setw(6)  << s.id
                  << std::setw(20) << s.name
                  << s.description << "\n";
    }
    std::cout << "\n";
}

void SampleView::showSample(const Sample& sample) const
{
    std::cout << "\n현재 정보"
              << "\n  ID  : " << sample.id
              << "\n  이름: " << sample.name
              << "\n  설명: " << sample.description << "\n\n";
}
