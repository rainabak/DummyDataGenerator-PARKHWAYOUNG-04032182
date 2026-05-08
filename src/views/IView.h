#pragma once
#include <string>
#include <vector>
#include "../models/Item.h"

class IView
{
public:
    virtual ~IView() = default;

    virtual void showMenu()                                = 0;
    virtual int  getMenuInput()                            = 0;
    virtual void showItems(const std::vector<Item>& items) = 0;
    virtual void showMessage(const std::string& message)   = 0;
    virtual Item getItemInput()                            = 0;
    virtual int  getIdInput()                              = 0;
};
