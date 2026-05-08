#include "MonitoringController.h"
#include "../utils/ConsoleUtil.h"

MonitoringController::MonitoringController(MonitoringView& view, MonitoringService& service)
    : m_view(view)
    , m_service(service)
{
}

void MonitoringController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();

        int choice = m_view.getMenuChoice();
        if (choice == 0) break;

        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void MonitoringController::handleChoice(int choice)
{
    switch (choice)
    {
    case 1: handleDashboard();    break;
    case 2: handleOrderSummary(); break;
    case 3: handleSampleList();   break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
}

void MonitoringController::handleDashboard()
{
    const auto summary  = m_service.getOrderSummary();
    const auto statuses = m_service.getSampleStatuses();
    m_view.showDashboard(summary, statuses);
}

void MonitoringController::handleOrderSummary()
{
    const auto summary = m_service.getOrderSummary();
    m_view.showOrderSummary(summary);
}

void MonitoringController::handleSampleList()
{
    const auto statuses = m_service.getSampleStatuses();
    m_view.showSampleList(statuses);
}
