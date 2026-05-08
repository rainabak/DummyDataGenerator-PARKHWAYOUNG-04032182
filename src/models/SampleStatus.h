#pragma once
#include <string>

enum class StockStatus
{
    SURPLUS,   // 여유: stock > demand
    SHORTAGE,  // 부족: 0 < stock <= demand
    DEPLETED   // 고갈: stock == 0
};

struct SampleStatus
{
    int         id;
    std::string name;
    std::string description;
    int         stock;
    int         demand;
    StockStatus stockStatus;
};
