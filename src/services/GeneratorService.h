#pragma once
#include "../generators/GeneratorConfig.h"
#include "../generators/SampleGenerator.h"
#include "../generators/OrderGenerator.h"
#include "../generators/ProductionLineGenerator.h"
#include "../persistence/JsonFileWriter.h"

class GeneratorService
{
public:
    GeneratorService(JsonFileWriter&          writer,
                     SampleGenerator&         sampleGen,
                     OrderGenerator&          orderGen,
                     ProductionLineGenerator& productionLineGen);

    void generateSamples(const GeneratorConfig& config);
    bool generateOrders(const GeneratorConfig& config);
    bool generateProductionLines(const GeneratorConfig& config);
    void generateAll(const GeneratorConfig& sampleConfig,
                     const GeneratorConfig& orderConfig,
                     const GeneratorConfig& lineConfig);

private:
    JsonFileWriter&          m_writer;
    SampleGenerator&         m_sampleGen;
    OrderGenerator&          m_orderGen;
    ProductionLineGenerator& m_productionLineGen;
};
