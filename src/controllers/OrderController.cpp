#include "OrderController.h"
#include "../utils/ConsoleUtil.h"

OrderController::OrderController(OrderView& view, IOrderRepository& repo)
    : m_view(view)
    , m_repo(repo)
{
}

void OrderController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();

        int choice = m_view.getMenuChoice();
        if (choice == 0) break;

        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void OrderController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleCreate(); break;
    case 2: handleList();   break;
    case 3: handleUpdate(); break;
    case 4: handleDelete(); break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void OrderController::handleCreate()
{
    Order o;
    o.productName = m_view.promptProductName();
    o.quantity    = m_view.promptQuantity();
    o.status      = m_view.promptStatus();

    m_repo.add(o);
    m_view.showMessage("주문이 등록되었습니다.");
}

void OrderController::handleList()
{
    const auto orders = m_repo.findAll();
    if (orders.empty())
    {
        m_view.showMessage("등록된 주문이 없습니다.");
        return;
    }
    m_view.showOrders(orders);
}

void OrderController::handleUpdate()
{
    const auto orders = m_repo.findAll();
    if (orders.empty())
    {
        m_view.showMessage("수정할 주문이 없습니다.");
        return;
    }
    m_view.showOrders(orders);

    const int id = m_view.promptId();
    Order* found = m_repo.findById(id);
    if (!found)
    {
        m_view.showMessage("해당 ID의 주문을 찾을 수 없습니다.");
        return;
    }

    m_view.showOrder(*found);

    Order updated;
    updated.id          = found->id;
    updated.sampleId    = found->sampleId;
    updated.customerName = found->customerName;
    updated.productName = m_view.promptProductName();
    updated.quantity    = m_view.promptQuantity();
    updated.status      = m_view.promptStatus();

    if (m_repo.update(updated))
        m_view.showMessage("주문이 수정되었습니다.");
    else
        m_view.showMessage("수정에 실패했습니다.");
}

void OrderController::handleDelete()
{
    const auto orders = m_repo.findAll();
    if (orders.empty())
    {
        m_view.showMessage("삭제할 주문이 없습니다.");
        return;
    }
    m_view.showOrders(orders);

    const int id = m_view.promptId();
    if (m_repo.remove(id))
        m_view.showMessage("주문이 삭제되었습니다.");
    else
        m_view.showMessage("해당 ID의 주문을 찾을 수 없습니다.");
}
