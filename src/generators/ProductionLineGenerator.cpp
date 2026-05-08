#include "ProductionLineGenerator.h"
#include <string>
#include <vector>

namespace
{
    static const std::vector<std::string> kLineStatuses = {
        "IDLE", "RUNNING", "PAUSED", "DONE",
    };

    static const int kProgressMin = 0;
    static const int kProgressMax = 99;   // DONE이면 100으로 고정
    static const int kProgressDone = 100;
}

ProductionLineGenerator::ProductionLineGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<ProductionLine> ProductionLineGenerator::generate(const GeneratorConfig& config,
                                                               int                    startId,
                                                               int                    orderIdMin,
                                                               int                    orderIdMax) const
{
    std::vector<ProductionLine> lines;
    lines.reserve(config.count);

    for (int i = 0; i < config.count; ++i)
    {
        ProductionLine pl;
        pl.id       = startId + i;
        pl.orderId  = m_random.nextInt(orderIdMin, orderIdMax);
        pl.lineName = buildLineName();
        pl.status   = pickStatus();
        pl.progress = (pl.status == "DONE") ? kProgressDone
                                             : m_random.nextInt(kProgressMin, kProgressMax);
        lines.push_back(pl);
    }

    return lines;
}

std::string ProductionLineGenerator::buildLineName() const
{
    char letter = static_cast<char>('A' + m_random.nextInt(0, 25));
    int  digits = m_random.nextInt(0, 99);
    std::string digitStr = (digits < 10 ? "0" : "") + std::to_string(digits);
    return std::string("LINE-") + letter + digitStr;
}

std::string ProductionLineGenerator::pickStatus() const
{
    return m_random.pickOne(kLineStatuses);
}
