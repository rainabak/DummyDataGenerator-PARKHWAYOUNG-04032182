#include "GeneratorService.h"
#include <map>

GeneratorService::GeneratorService(JsonFileWriter&          writer,
                                   SampleGenerator&         sampleGen,
                                   OrderGenerator&          orderGen,
                                   ProductionLineGenerator& productionLineGen)
    : m_writer(writer)
    , m_sampleGen(sampleGen)
    , m_orderGen(orderGen)
    , m_productionLineGen(productionLineGen)
{
}

// ── public ───────────────────────────────────────────────────────────────────

void GeneratorService::generateSamples(const GeneratorConfig& config)
{
    const int startId  = config.overwrite ? 1 : m_writer.readNextId("samples.json");
    const auto samples = m_sampleGen.generate(config, startId);
    const int nextId   = startId + static_cast<int>(samples.size());
    m_writer.writeSamples(samples, nextId, !config.overwrite);
}

bool GeneratorService::generateOrders(const GeneratorConfig& config)
{
    const int maxSampleId = m_writer.readNextId("samples.json") - 1;
    if (maxSampleId < 1) return false;

    const int startId  = config.overwrite ? 1 : m_writer.readNextId("orders.json");
    auto orders        = m_orderGen.generate(config, startId, 1, maxSampleId);

    // MonitoringService의 "order.productName == sample.name" 조건을 만족시키기 위해
    // sampleId에 해당하는 Sample 이름을 파일에서 읽어 productName에 반영한다.
    applyProductNames(orders, m_writer.readSampleNames());

    const int nextId = startId + static_cast<int>(orders.size());
    m_writer.writeOrders(orders, nextId, !config.overwrite);
    return true;
}

bool GeneratorService::generateProductionLines(const GeneratorConfig& config)
{
    const int maxOrderId = m_writer.readNextId("orders.json") - 1;
    if (maxOrderId < 1) return false;

    const int startId = config.overwrite ? 1 : m_writer.readNextId("production_lines.json");
    const auto lines  = m_productionLineGen.generate(config, startId, 1, maxOrderId);
    const int nextId  = startId + static_cast<int>(lines.size());
    m_writer.writeProductionLines(lines, nextId, !config.overwrite);
    return true;
}

void GeneratorService::generateAll(const GeneratorConfig& sampleConfig,
                                   const GeneratorConfig& orderConfig,
                                   const GeneratorConfig& lineConfig)
{
    // 1단계: Sample 생성
    const int sampleStartId = sampleConfig.overwrite ? 1 : m_writer.readNextId("samples.json");
    const auto samples      = m_sampleGen.generate(sampleConfig, sampleStartId);
    const int sampleNextId  = sampleStartId + static_cast<int>(samples.size());
    m_writer.writeSamples(samples, sampleNextId, !sampleConfig.overwrite);

    // 파일 저장 직후 읽어야 Append 모드의 기존 항목까지 포함한 전체 이름 맵을 얻는다.
    const auto sampleNames = m_writer.readSampleNames();

    // 2단계: Order 생성 (방금 확정된 Sample ID 범위 사용)
    const int maxSampleId  = sampleNextId - 1;
    const int orderStartId = orderConfig.overwrite ? 1 : m_writer.readNextId("orders.json");
    auto orders            = m_orderGen.generate(orderConfig, orderStartId, 1, maxSampleId);
    applyProductNames(orders, sampleNames);
    const int orderNextId  = orderStartId + static_cast<int>(orders.size());
    m_writer.writeOrders(orders, orderNextId, !orderConfig.overwrite);

    // 3단계: ProductionLine 생성 (방금 확정된 Order ID 범위 사용)
    const int maxOrderId  = orderNextId - 1;
    const int lineStartId = lineConfig.overwrite ? 1 : m_writer.readNextId("production_lines.json");
    const auto lines      = m_productionLineGen.generate(lineConfig, lineStartId, 1, maxOrderId);
    const int lineNextId  = lineStartId + static_cast<int>(lines.size());
    m_writer.writeProductionLines(lines, lineNextId, !lineConfig.overwrite);
}

// ── private ──────────────────────────────────────────────────────────────────

void GeneratorService::applyProductNames(std::vector<Order>&                    orders,
                                         const std::map<int, std::string>& sampleNames)
{
    for (auto& o : orders)
    {
        const auto it = sampleNames.find(o.sampleId);
        if (it != sampleNames.end())
            o.productName = it->second;
    }
}
