#pragma once
#include "IController.h"
#include "../views/ProductionLineView.h"

class ProductionLineController : public IController
{
public:
    explicit ProductionLineController(ProductionLineView& view);
    void run() override;

private:
    ProductionLineView& m_view;

    void handleChoice(int choice);
};
