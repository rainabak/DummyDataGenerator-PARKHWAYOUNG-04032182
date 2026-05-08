#include "MainController.h"
#include "../utils/ConsoleUtil.h"
#include <iostream>

MainController::MainController(MainMenuView&  view,
                               IController&   sample,
                               IController&   order,
                               IController&   productionLine,
                               IController&   monitoring,
                               IController&   shipment)
    : m_view(view)
    , m_sample(sample)
    , m_order(order)
    , m_productionLine(productionLine)
    , m_monitoring(monitoring)
    , m_shipment(shipment)
{
}

void MainController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();

        int choice = m_view.getMenuChoice();

        if (choice == 0)
            break;

        handleChoice(choice);
    }
}

void MainController::handleChoice(int choice)
{
    switch (choice)
    {
    case 1: m_sample.run();         break;
    case 2: m_order.run();          break;
    case 3: m_productionLine.run(); break;
    case 4: m_monitoring.run();     break;
    case 5: m_shipment.run();       break;
    default:
        ConsoleUtil::printSeparator();
        std::cout << "잘못된 입력입니다.\n";
        ConsoleUtil::printSeparator();
        ConsoleUtil::pause();
        break;
    }
}
