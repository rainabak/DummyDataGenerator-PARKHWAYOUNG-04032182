#include "SampleGenerator.h"

SampleGenerator::SampleGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<Sample> SampleGenerator::generate(const GeneratorConfig& config, int startId) const
{
    return {};
}

std::string SampleGenerator::buildName() const
{
    return {};
}

std::string SampleGenerator::buildDescription() const
{
    return {};
}
