#include "ProductionLineGenerator.h"

ProductionLineGenerator::ProductionLineGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<ProductionLine> ProductionLineGenerator::generate(const GeneratorConfig& config,
                                                               int                    startId,
                                                               int                    orderIdMin,
                                                               int                    orderIdMax) const
{
    return {};
}

std::string ProductionLineGenerator::buildLineName() const
{
    return {};
}

std::string ProductionLineGenerator::pickStatus() const
{
    return {};
}
