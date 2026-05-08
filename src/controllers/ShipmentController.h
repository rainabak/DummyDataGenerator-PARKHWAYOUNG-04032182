#pragma once
#include "IController.h"
#include "../views/ShipmentView.h"

class ShipmentController : public IController
{
public:
    explicit ShipmentController(ShipmentView& view);
    void run() override;

private:
    ShipmentView& m_view;

    void handleChoice(int choice);
};
