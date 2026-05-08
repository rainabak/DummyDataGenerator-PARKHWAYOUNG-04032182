#include "JsonFileWriter.h"

JsonFileWriter::JsonFileWriter(const std::string& dataDir)
    : m_dataDir(dataDir)
{
}

void JsonFileWriter::writeSamples(const std::vector<Sample>& samples, int nextId) const
{
}

void JsonFileWriter::writeOrders(const std::vector<Order>& orders, int nextId) const
{
}

void JsonFileWriter::writeProductionLines(const std::vector<ProductionLine>& lines, int nextId) const
{
}

void JsonFileWriter::writeFile(const std::string& path, const std::string& json) const
{
}

std::string JsonFileWriter::buildJson(const std::string& items, int nextId) const
{
    return {};
}
