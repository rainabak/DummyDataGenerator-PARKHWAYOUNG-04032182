#pragma once
#include <string>

struct Order
{
    int         id           { 0 };
    int         sampleId     { 0 };
    std::string productName;
    std::string customerName;
    int         quantity     { 0 };
    std::string status;
};
