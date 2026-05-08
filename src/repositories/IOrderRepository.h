#pragma once
#include <vector>
#include "../models/Order.h"

class IOrderRepository
{
public:
    virtual ~IOrderRepository() = default;

    virtual void               add(const Order& order)    = 0;
    virtual std::vector<Order> findAll()            const = 0;
    virtual Order*             findById(int id)           = 0;
    virtual bool               update(const Order& order) = 0;
    virtual bool               remove(int id)             = 0;
};
