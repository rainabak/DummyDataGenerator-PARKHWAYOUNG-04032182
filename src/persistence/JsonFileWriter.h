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

    void writeSamples(const std::vector<Sample>&              samples, int nextId, bool append = false) const;
    void writeOrders(const std::vector<Order>&               orders,  int nextId, bool append = false) const;
    void writeProductionLines(const std::vector<ProductionLine>& lines, int nextId, bool append = false) const;

    int  readNextId(const std::string& filename) const;

private:
    std::string m_dataDir;

    void        writeFile(const std::string& path, const std::string& json) const;
    std::string buildJson(const std::string& items, int nextId)             const;
    void        ensureDataDir()                                             const;

    std::string serializeSample(const Sample& s)              const;
    std::string serializeOrder(const Order& o)                const;
    std::string serializeProductionLine(const ProductionLine& pl) const;
};
