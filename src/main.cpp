#include <windows.h>

// ── 기존 레이어 ──────────────────────────────────────────────────
#include "utils/ConsoleUtil.h"
#include "views/MainMenuView.h"
#include "views/SampleView.h"
#include "views/OrderView.h"
#include "views/ProductionLineView.h"
#include "views/MonitoringView.h"
#include "views/ShipmentView.h"
#include "controllers/MainController.h"
#include "controllers/SampleController.h"
#include "controllers/OrderController.h"
#include "controllers/ProductionLineController.h"
#include "controllers/MonitoringController.h"
#include "controllers/ShipmentController.h"
#include "persistence/JsonFileStorage.h"
#include "repositories/SampleRepository.h"
#include "repositories/OrderRepository.h"
#include "services/MonitoringService.h"

// ── DummyDataGenerator 레이어 ────────────────────────────────────
#include "utils/RandomUtil.h"
#include "generators/SampleGenerator.h"
#include "generators/OrderGenerator.h"
#include "generators/ProductionLineGenerator.h"
#include "persistence/JsonFileWriter.h"
#include "services/GeneratorService.h"
#include "views/ConsoleView.h"
#include "controllers/DummyDataController.h"

int main()
{
    // ── Persistence ──────────────────────────────────────────────
    JsonFileStorage  sampleStorage("data/samples.json");
    JsonFileStorage  orderStorage("data/orders.json");
    SampleRepository sampleRepo(sampleStorage);
    OrderRepository  orderRepo(orderStorage);

    // ── Service ──────────────────────────────────────────────────
    MonitoringService monitoringService(orderRepo, sampleRepo);

    // ── DummyDataGenerator ───────────────────────────────────────
    RandomUtil              random;
    JsonFileWriter          writer("data");
    SampleGenerator         sampleGen(random);
    OrderGenerator          orderGen(random);
    ProductionLineGenerator productionLineGen(random);
    GeneratorService        generatorService(writer, sampleGen, orderGen, productionLineGen);
    ConsoleView             consoleView;
    DummyDataController     dummyCtrl(consoleView, generatorService);

    // ── View ─────────────────────────────────────────────────────
    MainMenuView       mainView;
    SampleView         sampleView;
    OrderView          orderView;
    ProductionLineView productionLineView;
    MonitoringView     monitoringView;
    ShipmentView       shipmentView;

    // ── Controller ───────────────────────────────────────────────
    SampleController         sampleCtrl(sampleView, sampleRepo);
    OrderController          orderCtrl(orderView, orderRepo);
    ProductionLineController productionLineCtrl(productionLineView);
    MonitoringController     monitoringCtrl(monitoringView, monitoringService);
    ShipmentController       shipmentCtrl(shipmentView);

    MainController controller(mainView,
                              sampleCtrl,
                              orderCtrl,
                              productionLineCtrl,
                              monitoringCtrl,
                              shipmentCtrl);

    // ── 최상위 메뉴 ──────────────────────────────────────────────
    while (true)
    {
        consoleView.showMainMenu();
        const int mode = consoleView.readMainMenuChoice();
        if      (mode == 0) break;
        else if (mode == 1) controller.run();
        else if (mode == 2) dummyCtrl.run();
    }

    return 0;
}
