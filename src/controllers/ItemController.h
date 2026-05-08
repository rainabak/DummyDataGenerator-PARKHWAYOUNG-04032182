#pragma once
#include "IController.h"
#include "../models/ItemRepository.h"
#include "../views/IView.h"

class ItemController : public IController
{
public:
    ItemController(ItemRepository& repository, IView& view);
    void run() override;

private:
    ItemRepository& m_repository;
    IView&          m_view;

    void handleAdd();
    void handleList();
    void handleUpdate();
    void handleDelete();
};
