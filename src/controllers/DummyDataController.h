#pragma once
#include "IController.h"
#include "../views/ConsoleView.h"
#include "../services/GeneratorService.h"

class DummyDataController : public IController
{
public:
    DummyDataController(ConsoleView& view, GeneratorService& service);
    void run() override;

private:
    ConsoleView&      m_view;
    GeneratorService& m_service;

    static constexpr int kMaxSample = 1000;
    static constexpr int kMaxOrder  = 10000;
    static constexpr int kMaxLine   = 10000;

    void handleSample();
    void handleOrder();
    void handleProductionLine();
    void handleAll();
};
