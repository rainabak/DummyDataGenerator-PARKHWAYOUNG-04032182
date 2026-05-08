#include "DummyDataController.h"

DummyDataController::DummyDataController(ConsoleView& view, GeneratorService& service)
    : m_view(view)
    , m_service(service)
{
}

void DummyDataController::run()
{
    while (true)
    {
        m_view.showGenerateMenu();
        const int choice = m_view.readMenuChoice();

        switch (choice)
        {
        case 0: return;
        case 1: handleSample();         break;
        case 2: handleOrder();          break;
        case 3: handleProductionLine(); break;
        case 4: handleAll();            break;
        }
    }
}

// ── private ──────────────────────────────────────────────────────────────────

void DummyDataController::handleSample()
{
    const int  count     = m_view.readCount(kMaxSample);
    const bool overwrite = m_view.readOverwrite();
    if (overwrite) m_view.showOverwriteWarning();

    m_service.generateSamples({ count, overwrite });
    m_view.showResult("Sample", count, "data/samples.json");
    m_view.pause();
}

void DummyDataController::handleOrder()
{
    const int  count     = m_view.readCount(kMaxOrder);
    const bool overwrite = m_view.readOverwrite();
    if (overwrite) m_view.showOverwriteWarning();

    if (!m_service.generateOrders({ count, overwrite }))
        m_view.showError("Order 생성 실패 — 먼저 Sample 데이터를 생성하세요.");
    else
        m_view.showResult("Order", count, "data/orders.json");

    m_view.pause();
}

void DummyDataController::handleProductionLine()
{
    const int  count     = m_view.readCount(kMaxLine);
    const bool overwrite = m_view.readOverwrite();
    if (overwrite) m_view.showOverwriteWarning();

    if (!m_service.generateProductionLines({ count, overwrite }))
        m_view.showError("ProductionLine 생성 실패 — 먼저 Order 데이터를 생성하세요.");
    else
        m_view.showResult("ProductionLine", count, "data/production_lines.json");

    m_view.pause();
}

void DummyDataController::handleAll()
{
    const int sampleCount = m_view.readCount(kMaxSample, "Sample");
    const int orderCount  = m_view.readCount(kMaxOrder,  "Order");
    const int lineCount   = m_view.readCount(kMaxLine,   "ProductionLine");
    const bool overwrite  = m_view.readOverwrite();
    if (overwrite) m_view.showOverwriteWarning();

    m_service.generateAll({ sampleCount, overwrite },
                          { orderCount,  overwrite },
                          { lineCount,   overwrite });

    m_view.showResult("Sample",         sampleCount, "data/samples.json");
    m_view.showResult("Order",          orderCount,  "data/orders.json");
    m_view.showResult("ProductionLine", lineCount,   "data/production_lines.json");
    m_view.pause();
}
