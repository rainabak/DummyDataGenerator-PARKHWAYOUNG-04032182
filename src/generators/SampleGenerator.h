#pragma once
#include <string>
#include <vector>
#include "../models/Sample.h"
#include "../utils/RandomUtil.h"
#include "GeneratorConfig.h"

class SampleGenerator
{
public:
    explicit SampleGenerator(RandomUtil& random);

    std::vector<Sample> generate(const GeneratorConfig& config, int startId) const;

private:
    RandomUtil& m_random;

    std::string buildName()        const;
    std::string buildDescription() const;
};
