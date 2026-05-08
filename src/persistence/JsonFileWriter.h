#pragma once
#include <string>
#include <vector>
#include "../models/Sample.h"
#include "../models/Order.h"
#include "../models/ProductionLine.h"

class JsonFileWriter
{
public:
    explicit JsonFileWriter(const std::string& dataDir);

    void writeSamples(const std::vector<Sample>&         samples, int nextId) const;
    void writeOrders(const std::vector<Order>&           orders,  int nextId) const;
    void writeProductionLines(const std::vector<ProductionLine>& lines,   int nextId) const;

private:
    std::string m_dataDir;

    void        writeFile(const std::string& path, const std::string& json) const;
    std::string buildJson(const std::string& items, int nextId)             const;
};
