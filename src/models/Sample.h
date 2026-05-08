#pragma once
#include <string>

struct Sample
{
    int         id                { 0 };
    std::string name;
    std::string description;
    int         stock             { 0 };
    int         avgProductionTime { 0 };
    double      yield             { 0.0 };
};
