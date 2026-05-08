#include "MonitoringView.h"
#include <iostream>
#include <limits>
#include <string>

static const std::string SEP   = "============================================\n";
static const std::string SEP_S = "--------------------------------------------\n";

// ── 표시 헬퍼 (이 파일 전용) ──────────────────────────────────────────────────

static std::string toStatusLabel(StockStatus status)
{
    switch (status)
    {
    case StockStatus::SURPLUS:
        return std::string("여유");

    case StockStatus::SHORTAGE:
        return std::string("부족");

    case StockStatus::DEPLETED:
        return std::string("고갈");

    default:
        return std::string("-");
    }
}

static void printHeader()
{
    std::cout << SEP;
    std::cout << "         [Data Monitoring Tool]\n";
    std::cout << SEP;
}

static void printOrderRows(const OrderSummary& summary)
{
    std::cout << "\n주문 상태 요약\n";
    std::cout << "- RESERVED  : " << summary.countReserved  << "건\n";
    std::cout << "- PRODUCING : " << summary.countProducing << "건\n";
    std::cout << "- CONFIRMED : " << summary.countConfirmed << "건\n";
    std::cout << "- RELEASE   : " << summary.countRelease   << "건\n";
}

static void printSampleItems(const std::vector<SampleStatus>& statuses)
{
    if (statuses.empty())
    {
        std::cout << "  등록된 시료가 없습니다.\n";
        return;
    }
    for (const auto& s : statuses)
    {
        std::cout << "- " << s.name
                  << " / " << s.description
                  << " / 재고 "  << s.stock
                  << " / 상태 "  << toStatusLabel(s.stockStatus)
                  << "\n";
    }
}

static void printFooter()
{
    std::cout << "\n" << SEP;
    std::cout << "  0. 뒤로가기\n";
}

// ── 메뉴 / 입력 ───────────────────────────────────────────────────────────────

void MonitoringView::showMenu() const
{
    std::cout << SEP;
    std::cout << "              [ 모니터링 ]\n";
    std::cout << SEP;
    std::cout << "  1. 전체 Dashboard\n";
    std::cout << "  2. 주문 상태별 집계\n";
    std::cout << "  3. 시료 재고 현황\n";
    std::cout << "  0. 돌아가기\n";
    std::cout << SEP;
    std::cout << "선택: ";
}

int MonitoringView::getMenuChoice() const
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

void MonitoringView::showMessage(const std::string& msg) const
{
    std::cout << "\n" << msg << "\n";
}

// ── 화면 출력 ─────────────────────────────────────────────────────────────────

void MonitoringView::showOrderSummary(const OrderSummary& summary) const
{
    printHeader();
    printOrderRows(summary);
    std::cout << SEP_S;
    std::cout << "  합계      : " << summary.total << "건\n";
    printFooter();
}

void MonitoringView::showSampleList(const std::vector<SampleStatus>& statuses) const
{
    printHeader();
    std::cout << "\n시료별 재고 현황\n";
    printSampleItems(statuses);
    printFooter();
}

void MonitoringView::showDashboard(const OrderSummary&              summary,
                                   const std::vector<SampleStatus>& statuses) const
{
    printHeader();
    printOrderRows(summary);
    std::cout << "\n" << SEP_S;
    std::cout << "시료별 재고 현황\n";
    printSampleItems(statuses);
    printFooter();
}
