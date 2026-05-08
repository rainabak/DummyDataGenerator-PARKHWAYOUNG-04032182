#pragma once
#include <map>
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

    int  readNextId(const std::string& filename)  const;

    // sampleId → name 맵 반환: GeneratorService의 productName 보정에 사용
    std::map<int, std::string> readSampleNames() const;

private:
    std::string m_dataDir;

    void        writeFile(const std::string& path, const std::string& json) const;
    std::string buildJson(const std::string& items, int nextId)             const;
    void        ensureDataDir()                                             const;
    void        logSaveStart(const std::string& label,
                              size_t count, bool append)                    const;
    void        logSaveComplete(const std::string& path)                    const;

    std::string serializeSample(const Sample& s)              const;
    std::string serializeOrder(const Order& o)                const;
    std::string serializeProductionLine(const ProductionLine& pl) const;
};
