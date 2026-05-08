#include "SampleGenerator.h"
#include <string>
#include <vector>

namespace
{
    static const std::vector<std::string> kDescriptions = {
        "A타입 반도체 검사 플랜",
        "B타입 고속 처리 플랜",
        "C타입 저전력 플랜",
        "D타입 고밀도 집적 플랜",
        "E타입 신뢰성 검사 플랜",
        "F타입 내열 환경 검사 플랜",
        "G타입 초정밀 측정 플랜",
        "H타입 다층 기판 검사 플랜",
    };

    static const int kStockMin        = 0;
    static const int kStockMax        = 50;
    static const int kAvgTimeMin      = 2;
    static const int kAvgTimeMax      = 12;
    static const int kYieldMinX10     = 600;  // 60.0
    static const int kYieldMaxX10     = 999;  // 99.9
}

SampleGenerator::SampleGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<Sample> SampleGenerator::generate(const GeneratorConfig& config, int startId) const
{
    std::vector<Sample> samples;
    samples.reserve(config.count);

    for (int i = 0; i < config.count; ++i)
    {
        Sample s;
        s.id                = startId + i;
        s.name              = buildName();
        s.description       = buildDescription();
        s.stock             = m_random.nextInt(kStockMin, kStockMax);
        s.avgProductionTime = m_random.nextInt(kAvgTimeMin, kAvgTimeMax);
        s.yield             = m_random.nextInt(kYieldMinX10, kYieldMaxX10) / 10.0;
        samples.push_back(s);
    }

    return samples;
}

std::string SampleGenerator::buildName() const
{
    char c1 = static_cast<char>('A' + m_random.nextInt(0, 25));
    char c2 = static_cast<char>('A' + m_random.nextInt(0, 25));
    return std::string("플랜-") + c1 + c2 + "샘";
}

std::string SampleGenerator::buildDescription() const
{
    return m_random.pickOne(kDescriptions);
}
