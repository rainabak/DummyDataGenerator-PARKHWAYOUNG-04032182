#pragma once
#include "IController.h"
#include "../views/MainMenuView.h"

class MainController : public IController
{
public:
    MainController(MainMenuView&  view,
                   IController&   sample,
                   IController&   order,
                   IController&   productionLine,
                   IController&   monitoring,
                   IController&   shipment);
    void run() override;

private:
    MainMenuView& m_view;
    IController&  m_sample;
    IController&  m_order;
    IController&  m_productionLine;
    IController&  m_monitoring;
    IController&  m_shipment;

    void handleChoice(int choice);
};
