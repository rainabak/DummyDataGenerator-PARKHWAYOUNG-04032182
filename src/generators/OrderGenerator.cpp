#include "OrderGenerator.h"

OrderGenerator::OrderGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<Order> OrderGenerator::generate(const GeneratorConfig& config,
                                             int                    startId,
                                             int                    sampleIdMin,
                                             int                    sampleIdMax) const
{
    return {};
}

std::string OrderGenerator::buildCustomerName() const
{
    return {};
}

std::string OrderGenerator::pickStatus() const
{
    return {};
}
