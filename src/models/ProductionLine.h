#pragma once
#include <string>

struct ProductionLine
{
    int         id       { 0 };
    int         orderId  { 0 };
    std::string lineName;
    std::string status;
    int         progress { 0 };
};
