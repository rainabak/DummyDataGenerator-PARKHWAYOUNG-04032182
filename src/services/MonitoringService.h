#pragma once
#include <vector>
#include "../models/OrderSummary.h"
#include "../models/SampleStatus.h"
#include "../repositories/IOrderRepository.h"
#include "../repositories/ISampleRepository.h"

class MonitoringService
{
public:
    MonitoringService(IOrderRepository& orderRepo, ISampleRepository& sampleRepo);

    OrderSummary              getOrderSummary()   const;
    std::vector<SampleStatus> getSampleStatuses() const;

private:
    IOrderRepository&  m_orderRepo;
    ISampleRepository& m_sampleRepo;
};
