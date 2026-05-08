#pragma once
#include <string>
#include <vector>
#include "../models/Order.h"
#include "../utils/RandomUtil.h"
#include "GeneratorConfig.h"

class OrderGenerator
{
public:
    explicit OrderGenerator(RandomUtil& random);

    std::vector<Order> generate(const GeneratorConfig& config,
                                int                    startId,
                                int                    sampleIdMin,
                                int                    sampleIdMax) const;

private:
    RandomUtil& m_random;

    std::string buildCustomerName() const;
    std::string pickStatus()        const;
};
