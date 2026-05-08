#pragma once
#include "IController.h"
#include "../views/OrderView.h"
#include "../repositories/IOrderRepository.h"

class OrderController : public IController
{
public:
    OrderController(OrderView& view, IOrderRepository& repo);
    void run() override;

private:
    OrderView&        m_view;
    IOrderRepository& m_repo;

    void handleChoice(int choice);
    void handleCreate();
    void handleList();
    void handleUpdate();
    void handleDelete();
};
