#include "GeneratorService.h"

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

void GeneratorService::generateSamples(const GeneratorConfig& config)
{
    const int startId = config.overwrite ? 1 : m_writer.readNextId("samples.json");
    const auto samples = m_sampleGen.generate(config, startId);
    const int nextId = startId + static_cast<int>(samples.size());
    m_writer.writeSamples(samples, nextId, !config.overwrite);
}

bool GeneratorService::generateOrders(const GeneratorConfig& config)
{
    const int maxSampleId = m_writer.readNextId("samples.json") - 1;
    if (maxSampleId < 1) return false;

    const int startId = config.overwrite ? 1 : m_writer.readNextId("orders.json");
    const auto orders = m_orderGen.generate(config, startId, 1, maxSampleId);
    const int nextId  = startId + static_cast<int>(orders.size());
    m_writer.writeOrders(orders, nextId, !config.overwrite);
    return true;
}

bool GeneratorService::generateProductionLines(const GeneratorConfig& config)
{
    const int maxOrderId = m_writer.readNextId("orders.json") - 1;
    if (maxOrderId < 1) return false;

    const int startId = config.overwrite ? 1 : m_writer.readNextId("production_lines.json");
    const auto lines = m_productionLineGen.generate(config, startId, 1, maxOrderId);
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

    // 2단계: Order 생성 (방금 생성된 Sample ID 범위 사용)
    const int maxSampleId  = sampleNextId - 1;
    const int orderStartId = orderConfig.overwrite ? 1 : m_writer.readNextId("orders.json");
    const auto orders      = m_orderGen.generate(orderConfig, orderStartId, 1, maxSampleId);
    const int orderNextId  = orderStartId + static_cast<int>(orders.size());
    m_writer.writeOrders(orders, orderNextId, !orderConfig.overwrite);

    // 3단계: ProductionLine 생성 (방금 생성된 Order ID 범위 사용)
    const int maxOrderId  = orderNextId - 1;
    const int lineStartId = lineConfig.overwrite ? 1 : m_writer.readNextId("production_lines.json");
    const auto lines      = m_productionLineGen.generate(lineConfig, lineStartId, 1, maxOrderId);
    const int lineNextId  = lineStartId + static_cast<int>(lines.size());
    m_writer.writeProductionLines(lines, lineNextId, !lineConfig.overwrite);
}
