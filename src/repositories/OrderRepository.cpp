#include "OrderRepository.h"
#include "../persistence/JsonUtil.h"
#include <algorithm>
#include <sstream>

// ── Order 직렬화/역직렬화 (이 파일 전용) ─────────────────────────────────────

static std::string toJson(const Order& o)
{
    std::ostringstream oss;
    oss << "{"
        << "\"id\":"            << o.id                                  << ","
        << "\"productName\":\"" << JsonUtil::escapeString(o.productName) << "\","
        << "\"quantity\":"      << o.quantity                            << ","
        << "\"status\":\""      << JsonUtil::escapeString(o.status)      << "\""
        << "}";
    return oss.str();
}

static std::string buildJson(const std::vector<Order>& orders, int nextId)
{
    std::ostringstream oss;
    oss << "{\"nextId\":" << nextId << ",\"items\":[";
    for (size_t i = 0; i < orders.size(); i++)
    {
        if (i > 0) oss << ",";
        oss << toJson(orders[i]);
    }
    oss << "]}";
    return oss.str();
}

static Order fromJson(const std::string& obj)
{
    return Order
    {
        JsonUtil::readInt(obj,    "id"),
        JsonUtil::readString(obj, "productName"),
        JsonUtil::readInt(obj,    "quantity"),
        JsonUtil::readString(obj, "status")
    };
}

static void loadFromJson(const std::string& raw,
                         std::vector<Order>& out,
                         int& nextId)
{
    out.clear();
    nextId = 1;
    if (raw.empty()) return;

    nextId = JsonUtil::readInt(raw, "nextId");

    for (const auto& obj : JsonUtil::splitObjects(raw))
    {
        Order o = fromJson(obj);
        if (o.id > 0)
            out.push_back(std::move(o));
    }

    for (const auto& o : out)
    {
        if (o.id >= nextId)
            nextId = o.id + 1;
    }
}

// ── OrderRepository ───────────────────────────────────────────────────────────

OrderRepository::OrderRepository(JsonFileStorage& storage)
    : m_storage(storage)
{
    loadFromJson(m_storage.load(), m_orders, m_nextId);
}

void OrderRepository::add(const Order& order)
{
    m_orders.push_back({ m_nextId++, order.productName, order.quantity, order.status });
    persist();
}

std::vector<Order> OrderRepository::findAll() const
{
    if (!m_storage.exists())
        return {};

    std::vector<Order> result;
    int nextId;
    loadFromJson(m_storage.load(), result, nextId);
    return result;
}

Order* OrderRepository::findById(int id)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [id](const Order& o) { return o.id == id; });
    return it != m_orders.end() ? &(*it) : nullptr;
}

bool OrderRepository::update(const Order& order)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [&order](const Order& o) { return o.id == order.id; });
    if (it == m_orders.end()) return false;

    *it = order;
    persist();
    return true;
}

bool OrderRepository::remove(int id)
{
    auto it = std::find_if(m_orders.begin(), m_orders.end(),
                           [id](const Order& o) { return o.id == id; });
    if (it == m_orders.end()) return false;

    m_orders.erase(it);
    persist();
    return true;
}

void OrderRepository::persist() const
{
    m_storage.save(buildJson(m_orders, m_nextId));
}
