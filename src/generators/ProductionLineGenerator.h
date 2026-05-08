#pragma once
#include <string>
#include <vector>
#include "../models/ProductionLine.h"
#include "../utils/RandomUtil.h"
#include "GeneratorConfig.h"

class ProductionLineGenerator
{
public:
    explicit ProductionLineGenerator(RandomUtil& random);

    std::vector<ProductionLine> generate(const GeneratorConfig& config,
                                         int                    startId,
                                         int                    orderIdMin,
                                         int                    orderIdMax) const;

private:
    RandomUtil& m_random;

    std::string buildLineName() const;
    std::string pickStatus()    const;
};
