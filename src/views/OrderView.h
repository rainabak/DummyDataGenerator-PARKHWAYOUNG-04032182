#pragma once
#include <string>
#include <vector>
#include "../models/Order.h"

class OrderView
{
public:
    void        showMenu()                              const;
    int         getMenuChoice()                         const;
    void        showMessage(const std::string& msg)     const;

    std::string promptProductName()                     const;
    int         promptQuantity()                        const;
    std::string promptStatus()                          const;
    int         promptId()                              const;
    void        showOrders(const std::vector<Order>&)   const;
    void        showOrder(const Order&)                 const;
};
