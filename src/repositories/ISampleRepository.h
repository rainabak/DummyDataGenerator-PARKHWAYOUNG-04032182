#pragma once
#include <vector>
#include "../models/Sample.h"

class ISampleRepository
{
public:
    virtual ~ISampleRepository() = default;

    virtual void                add(const Sample& sample)    = 0;
    virtual std::vector<Sample> findAll()              const = 0;
    virtual Sample*             findById(int id)             = 0;
    virtual bool                update(const Sample& sample) = 0;
    virtual bool                remove(int id)               = 0;
};
