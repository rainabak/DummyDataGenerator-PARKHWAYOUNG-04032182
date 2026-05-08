#pragma once
#include "IController.h"
#include "../views/MonitoringView.h"
#include "../services/MonitoringService.h"

class MonitoringController : public IController
{
public:
    MonitoringController(MonitoringView& view, MonitoringService& service);
    void run() override;

private:
    MonitoringView&    m_view;
    MonitoringService& m_service;

    void handleChoice(int choice);
    void handleDashboard();
    void handleOrderSummary();
    void handleSampleList();
};
