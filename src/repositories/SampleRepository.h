#pragma once
#include <vector>
#include "ISampleRepository.h"
#include "../persistence/JsonFileStorage.h"

class SampleRepository : public ISampleRepository
{
public:
    explicit SampleRepository(JsonFileStorage& storage);

    void                add(const Sample& sample)    override;
    std::vector<Sample> findAll()              const override;
    Sample*             findById(int id)             override;
    bool                update(const Sample& sample) override;
    bool                remove(int id)               override;

private:
    JsonFileStorage&    m_storage;
    std::vector<Sample> m_samples;
    int                 m_nextId{ 1 };

    void persist() const;
};
