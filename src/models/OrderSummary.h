#pragma once

struct OrderSummary
{
    int countReserved  { 0 };
    int countProducing { 0 };
    int countConfirmed { 0 };
    int countRelease   { 0 };
    int total          { 0 };
};
