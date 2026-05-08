#include "JsonFileStorage.h"
#include <fstream>
#include <iostream>
#include <sstream>

JsonFileStorage::JsonFileStorage(const std::string& filePath)
    : m_path(filePath)
{
}

bool JsonFileStorage::exists() const
{
    return std::filesystem::exists(m_path);
}

std::string JsonFileStorage::load() const
{
    if (!exists())
        return "";

    std::string json;
    {
        std::ifstream file(m_path);
        if (!file.is_open())
        {
            std::cerr << "[오류] 파일을 열 수 없습니다: " << m_path.string() << "\n";
            return "";
        }
        std::ostringstream oss;
        oss << file.rdbuf();
        json = oss.str();
    } // ifstream이 여기서 닫힘 — rename 전에 핸들 해제

    if (json.empty())
        return "";

    // UTF-8 BOM(EF BB BF) 제거 — Visual Studio가 붙이는 경우 대응
    if (json.size() >= 3 &&
        static_cast<unsigned char>(json[0]) == 0xEF &&
        static_cast<unsigned char>(json[1]) == 0xBB &&
        static_cast<unsigned char>(json[2]) == 0xBF)
    {
        json.erase(0, 3);
    }

    if (!isValidJson(json))
    {
        std::cerr << "[오류] JSON 형식 오류 — 파일을 백업하고 초기화합니다: " << m_path.string() << "\n";
        backupCorrupted();
        return "";
    }

    return json;
}

void JsonFileStorage::save(const std::string& json) const
{
    ensureDirectory();

    std::filesystem::path tmpPath = m_path;
    tmpPath += ".tmp";

    {
        std::ofstream tmp(tmpPath, std::ios::out | std::ios::trunc);
        if (!tmp.is_open())
        {
            std::cerr << "[JsonFileStorage] 저장 실패 (임시 파일 생성 불가): " << tmpPath.string() << "\n";
            return;
        }
        tmp << json;
    }

    std::error_code ec;
    std::filesystem::rename(tmpPath, m_path, ec);
    if (ec)
    {
        std::cerr << "[JsonFileStorage] 저장 실패 (파일 교체 불가): " << ec.message() << "\n";
        std::filesystem::remove(tmpPath, ec);
    }
}

// ── private ──────────────────────────────────────────────────────────────────

void JsonFileStorage::ensureDirectory() const
{
    auto dir = m_path.parent_path();
    if (dir.empty()) return;

    std::error_code ec;
    std::filesystem::create_directories(dir, ec);
    if (ec)
    {
        std::cerr << "[JsonFileStorage] 디렉터리 생성 실패: " << dir.string()
                  << " (" << ec.message() << ")\n";
    }
}

bool JsonFileStorage::isValidJson(const std::string& json) const
{
    const auto first = json.find_first_not_of(" \t\r\n");
    const auto last  = json.find_last_not_of(" \t\r\n");

    if (first == std::string::npos || last == std::string::npos)
        return false;

    return json[first] == '{' && json[last] == '}';
}

void JsonFileStorage::backupCorrupted() const
{
    std::filesystem::path bakPath = m_path;
    bakPath += ".bak";

    std::error_code ec;
    std::filesystem::rename(m_path, bakPath, ec);

    if (ec)
        std::cerr << "[JsonFileStorage] 백업 실패: " << ec.message() << "\n";
    else
        std::cout << "[JsonFileStorage] 손상 파일 백업 완료: " << bakPath.string() << "\n";
}
