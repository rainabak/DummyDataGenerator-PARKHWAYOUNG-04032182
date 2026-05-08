#include <windows.h>
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

int main()
{
    // Persistence layer
    JsonFileStorage  sampleStorage("data/samples.json");
    JsonFileStorage  orderStorage("data/orders.json");
    SampleRepository sampleRepo(sampleStorage);
    OrderRepository  orderRepo(orderStorage);

    // Service layer
    MonitoringService monitoringService(orderRepo, sampleRepo);

    // Views
    MainMenuView       mainView;
    SampleView         sampleView;
    OrderView          orderView;
    ProductionLineView productionLineView;
    MonitoringView     monitoringView;
    ShipmentView       shipmentView;

    // Controllers
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
    controller.run();

    return 0;
}
