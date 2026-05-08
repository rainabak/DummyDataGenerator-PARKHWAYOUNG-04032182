#pragma once
#include <filesystem>
#include <string>

class JsonFileStorage
{
public:
    explicit JsonFileStorage(const std::string& filePath);

    std::string load()                        const;
    void        save(const std::string& json) const;
    bool        exists()                      const;

private:
    std::filesystem::path m_path;

    void ensureDirectory()                          const;
    bool isValidJson(const std::string& json)       const;
    void backupCorrupted()                          const;
};
