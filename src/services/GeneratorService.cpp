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
}

void GeneratorService::generateOrders(const GeneratorConfig& config)
{
}

void GeneratorService::generateProductionLines(const GeneratorConfig& config)
{
}

void GeneratorService::generateAll(const GeneratorConfig& sampleConfig,
                                   const GeneratorConfig& orderConfig,
                                   const GeneratorConfig& lineConfig)
{
}
