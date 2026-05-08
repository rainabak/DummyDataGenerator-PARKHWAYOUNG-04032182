#include "JsonFileWriter.h"
#include "JsonFileStorage.h"
#include "JsonUtil.h"
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace
{
    // items 문자열에 새 항목을 콤마 구분자와 함께 연결한다.
    void joinItem(std::string& items, const std::string& item)
    {
        if (!items.empty()) items += ",\n    ";
        items += item;
    }

    // 파일이 있으면 기존 항목을 raw JSON 문자열로 불러온다.
    std::string loadExistingItems(const std::string& path)
    {
        std::string items;
        for (const auto& raw : JsonUtil::splitObjects(JsonFileStorage(path).load()))
            joinItem(items, raw);
        return items;
    }
}

JsonFileWriter::JsonFileWriter(const std::string& dataDir)
    : m_dataDir(dataDir)
{
}

// ── public ───────────────────────────────────────────────────────────────────

void JsonFileWriter::writeSamples(const std::vector<Sample>& samples, int nextId, bool append) const
{
    const std::string path = m_dataDir + "/samples.json";
    logSaveStart("Sample", samples.size(), append);

    std::string items = append ? loadExistingItems(path) : "";
    for (const auto& s : samples)
        joinItem(items, serializeSample(s));

    writeFile(path, buildJson(items, nextId));
    logSaveComplete(path);
}

void JsonFileWriter::writeOrders(const std::vector<Order>& orders, int nextId, bool append) const
{
    const std::string path = m_dataDir + "/orders.json";
    logSaveStart("Order", orders.size(), append);

    std::string items = append ? loadExistingItems(path) : "";
    for (const auto& o : orders)
        joinItem(items, serializeOrder(o));

    writeFile(path, buildJson(items, nextId));
    logSaveComplete(path);
}

void JsonFileWriter::writeProductionLines(const std::vector<ProductionLine>& lines, int nextId, bool append) const
{
    const std::string path = m_dataDir + "/production_lines.json";
    logSaveStart("ProductionLine", lines.size(), append);

    std::string items = append ? loadExistingItems(path) : "";
    for (const auto& pl : lines)
        joinItem(items, serializeProductionLine(pl));

    writeFile(path, buildJson(items, nextId));
    logSaveComplete(path);
}

int JsonFileWriter::readNextId(const std::string& filename) const
{
    const std::string path = m_dataDir + "/" + filename;
    const std::string json = JsonFileStorage(path).load();
    if (json.empty()) return 1;
    const int nextId = JsonUtil::readInt(json, "nextId");
    return nextId > 0 ? nextId : 1;
}

std::map<int, std::string> JsonFileWriter::readSampleNames() const
{
    const std::string path = m_dataDir + "/samples.json";
    const std::string json = JsonFileStorage(path).load();

    std::map<int, std::string> result;
    for (const auto& raw : JsonUtil::splitObjects(json))
    {
        const int id = JsonUtil::readInt(raw, "id");
        if (id > 0)
            result[id] = JsonUtil::readString(raw, "name");
    }
    return result;
}

// ── private ──────────────────────────────────────────────────────────────────

void JsonFileWriter::writeFile(const std::string& path, const std::string& json) const
{
    ensureDataDir();

    const std::string tmpPath = path + ".tmp";
    {
        std::ofstream tmp(tmpPath, std::ios::out | std::ios::trunc);
        if (!tmp.is_open())
        {
            std::cerr << "[JsonFileWriter] 저장 실패 (임시 파일 생성 불가): " << tmpPath << "\n";
            return;
        }
        tmp << "\xEF\xBB\xBF";  // UTF-8 BOM
        tmp << json;
    }

    std::error_code ec;
    std::filesystem::rename(tmpPath, path, ec);
    if (ec)
    {
        std::cerr << "[JsonFileWriter] 저장 실패 (파일 교체 불가): " << ec.message() << "\n";
        std::filesystem::remove(tmpPath, ec);
    }
}

std::string JsonFileWriter::buildJson(const std::string& items, int nextId) const
{
    const std::string arr = items.empty() ? "[]"
                                          : "[\n    " + items + "\n  ]";
    return "{\n"
           "  \"nextId\": " + std::to_string(nextId) + ",\n"
           "  \"items\": "  + arr                     + "\n"
           "}";
}

void JsonFileWriter::ensureDataDir() const
{
    std::error_code ec;
    std::filesystem::create_directories(m_dataDir, ec);
    if (ec)
    {
        std::cerr << "[JsonFileWriter] 디렉터리 생성 실패: " << m_dataDir
                  << " (" << ec.message() << ")\n";
    }
}

void JsonFileWriter::logSaveStart(const std::string& label, size_t count, bool append) const
{
    std::cout << "[JsonFileWriter] " << label << " 저장 시작 — " << count << "건"
              << (append ? " (Append 모드)" : " (Overwrite 모드)") << "\n";
}

void JsonFileWriter::logSaveComplete(const std::string& path) const
{
    std::cout << "[JsonFileWriter] 저장 완료 — " << path << "\n";
}

std::string JsonFileWriter::serializeSample(const Sample& s) const
{
    std::ostringstream yieldStream;
    yieldStream << std::fixed << std::setprecision(1) << s.yield;

    return std::string("{ ")
        + "\"id\": "                 + std::to_string(s.id)
        + ", \"name\": \""           + JsonUtil::escapeString(s.name)            + "\""
        + ", \"description\": \""    + JsonUtil::escapeString(s.description)     + "\""
        + ", \"stock\": "            + std::to_string(s.stock)
        + ", \"avgProductionTime\": "+ std::to_string(s.avgProductionTime)
        + ", \"yield\": "            + yieldStream.str()
        + " }";
}

std::string JsonFileWriter::serializeOrder(const Order& o) const
{
    return std::string("{ ")
        + "\"id\": "               + std::to_string(o.id)
        + ", \"sampleId\": "       + std::to_string(o.sampleId)
        + ", \"productName\": \""  + JsonUtil::escapeString(o.productName)  + "\""
        + ", \"customerName\": \"" + JsonUtil::escapeString(o.customerName) + "\""
        + ", \"quantity\": "       + std::to_string(o.quantity)
        + ", \"status\": \""       + JsonUtil::escapeString(o.status)       + "\""
        + " }";
}

std::string JsonFileWriter::serializeProductionLine(const ProductionLine& pl) const
{
    return std::string("{ ")
        + "\"id\": "           + std::to_string(pl.id)
        + ", \"orderId\": "    + std::to_string(pl.orderId)
        + ", \"lineName\": \"" + JsonUtil::escapeString(pl.lineName) + "\""
        + ", \"status\": \""   + JsonUtil::escapeString(pl.status)   + "\""
        + ", \"progress\": "   + std::to_string(pl.progress)
        + " }";
}
