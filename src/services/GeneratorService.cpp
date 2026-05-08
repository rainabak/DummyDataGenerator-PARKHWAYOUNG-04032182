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
    const int startId  = resolveStartId(config, "samples.json");
    const auto samples = m_sampleGen.generate(config, startId);
    m_writer.writeSamples(samples, startId + static_cast<int>(samples.size()), !config.overwrite);
}

bool GeneratorService::generateOrders(const GeneratorConfig& config)
{
    const int maxSampleId = m_writer.readNextId("samples.json") - 1;
    if (maxSampleId < 1) return false;

    const int startId = resolveStartId(config, "orders.json");
    auto orders       = m_orderGen.generate(config, startId, 1, maxSampleId);
    applyProductNames(orders, m_writer.readSampleNames());
    m_writer.writeOrders(orders, startId + static_cast<int>(orders.size()), !config.overwrite);
    return true;
}

bool GeneratorService::generateProductionLines(const GeneratorConfig& config)
{
    const int maxOrderId = m_writer.readNextId("orders.json") - 1;
    if (maxOrderId < 1) return false;

    const int startId = resolveStartId(config, "production_lines.json");
    const auto lines  = m_productionLineGen.generate(config, startId, 1, maxOrderId);
    m_writer.writeProductionLines(lines, startId + static_cast<int>(lines.size()), !config.overwrite);
    return true;
}

void GeneratorService::generateAll(const GeneratorConfig& sampleConfig,
                                   const GeneratorConfig& orderConfig,
                                   const GeneratorConfig& lineConfig)
{
    // 1단계: Sample 생성
    const int sampleStart  = resolveStartId(sampleConfig, "samples.json");
    const auto samples     = m_sampleGen.generate(sampleConfig, sampleStart);
    const int sampleNextId = sampleStart + static_cast<int>(samples.size());
    m_writer.writeSamples(samples, sampleNextId, !sampleConfig.overwrite);

    // 파일 저장 직후 읽어야 Append 모드의 기존 항목까지 포함한 전체 이름 맵을 얻는다.
    const auto sampleNames = m_writer.readSampleNames();

    // 2단계: Order 생성 (방금 확정된 Sample ID 범위 사용)
    const int orderStart  = resolveStartId(orderConfig, "orders.json");
    auto orders           = m_orderGen.generate(orderConfig, orderStart, 1, sampleNextId - 1);
    applyProductNames(orders, sampleNames);
    const int orderNextId = orderStart + static_cast<int>(orders.size());
    m_writer.writeOrders(orders, orderNextId, !orderConfig.overwrite);

    // 3단계: ProductionLine 생성 (방금 확정된 Order ID 범위 사용)
    const int lineStart = resolveStartId(lineConfig, "production_lines.json");
    const auto lines    = m_productionLineGen.generate(lineConfig, lineStart, 1, orderNextId - 1);
    m_writer.writeProductionLines(lines, lineStart + static_cast<int>(lines.size()), !lineConfig.overwrite);
}

// ── private ──────────────────────────────────────────────────────────────────

int GeneratorService::resolveStartId(const GeneratorConfig& config,
                                     const std::string&     filename) const
{
    return config.overwrite ? 1 : m_writer.readNextId(filename);
}

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
