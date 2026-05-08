#pragma once
#include <vector>
#include "IOrderRepository.h"
#include "../persistence/JsonFileStorage.h"

class OrderRepository : public IOrderRepository
{
public:
    explicit OrderRepository(JsonFileStorage& storage);

    void               add(const Order& order)    override;
    std::vector<Order> findAll()            const override;
    Order*             findById(int id)           override;
    bool               update(const Order& order) override;
    bool               remove(int id)             override;

private:
    JsonFileStorage&   m_storage;
    std::vector<Order> m_orders;
    int                m_nextId{ 1 };

    void persist() const;
};
