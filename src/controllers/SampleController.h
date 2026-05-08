#pragma once
#include "IController.h"
#include "../views/SampleView.h"
#include "../repositories/ISampleRepository.h"

class SampleController : public IController
{
public:
    SampleController(SampleView& view, ISampleRepository& repo);
    void run() override;

private:
    SampleView&        m_view;
    ISampleRepository& m_repo;

    void handleChoice(int choice);
    void handleCreate();
    void handleList();
    void handleUpdate();
    void handleDelete();
};
