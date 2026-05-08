#pragma once
#include <vector>
#include "Item.h"

class ItemRepository
{
public:
    void              add(const Item& item);
    std::vector<Item> findAll() const;
    Item*             findById(int id);
    bool              update(const Item& item);
    bool              remove(int id);

private:
    std::vector<Item> m_items;
    int               m_nextId{ 1 };
};
