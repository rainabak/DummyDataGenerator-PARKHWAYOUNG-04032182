#pragma once
#include "IView.h"

class ItemView : public IView
{
public:
    void showMenu()                                override;
    int  getMenuInput()                            override;
    void showItems(const std::vector<Item>& items) override;
    void showMessage(const std::string& message)   override;
    Item getItemInput()                            override;
    int  getIdInput()                              override;
};
