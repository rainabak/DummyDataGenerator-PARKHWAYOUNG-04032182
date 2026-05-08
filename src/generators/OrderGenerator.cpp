#include "OrderGenerator.h"
#include <string>
#include <vector>

namespace
{
    const std::vector<std::string> kCustomerNames = {
        "삼성전자", "SK하이닉스", "인텔", "TSMC", "AMD",
        "마이크론", "퀄컴", "엔비디아", "도시바", "웨스턴디지털",
    };

    struct StatusWeight
    {
        const char* status;
        int         weight;
    };

    // 가중치 합계: 100
    const StatusWeight kOrderStatuses[] = {
        { "RESERVED",  30 },
        { "PRODUCING", 25 },
        { "CONFIRMED", 20 },
        { "RELEASE",   15 },
        { "REJECTED",  10 },
    };

    constexpr int kQuantityMin = 1;
    constexpr int kQuantityMax = 20;
}

OrderGenerator::OrderGenerator(RandomUtil& random)
    : m_random(random)
{
}

std::vector<Order> OrderGenerator::generate(const GeneratorConfig& config,
                                             int                    startId,
                                             int                    sampleIdMin,
                                             int                    sampleIdMax) const
{
    std::vector<Order> orders;
    orders.reserve(config.count);

    for (int i = 0; i < config.count; ++i)
    {
        Order o;
        o.id           = startId + i;
        o.sampleId     = m_random.nextInt(sampleIdMin, sampleIdMax);
        o.productName  = "제품-" + std::to_string(o.sampleId);
        o.customerName = buildCustomerName();
        o.quantity     = m_random.nextInt(kQuantityMin, kQuantityMax);
        o.status       = pickStatus();
        orders.push_back(o);
    }

    return orders;
}

std::string OrderGenerator::buildCustomerName() const
{
    return m_random.pickOne(kCustomerNames);
}

std::string OrderGenerator::pickStatus() const
{
    int total = 0;
    for (const auto& sw : kOrderStatuses) total += sw.weight;

    int roll       = m_random.nextInt(1, total);
    int cumulative = 0;
    for (const auto& sw : kOrderStatuses)
    {
        cumulative += sw.weight;
        if (roll <= cumulative) return sw.status;
    }
    return kOrderStatuses[0].status;
}
