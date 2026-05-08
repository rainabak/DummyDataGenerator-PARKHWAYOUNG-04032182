#include "SampleController.h"
#include "../utils/ConsoleUtil.h"

SampleController::SampleController(SampleView& view, ISampleRepository& repo)
    : m_view(view)
    , m_repo(repo)
{
}

void SampleController::run()
{
    while (true)
    {
        ConsoleUtil::clearScreen();
        m_view.showMenu();

        int choice = m_view.getMenuChoice();
        if (choice == 0) break;

        handleChoice(choice);
        ConsoleUtil::pause();
    }
}

void SampleController::handleChoice(int choice)
{
    ConsoleUtil::printSeparator();
    switch (choice)
    {
    case 1: handleCreate(); break;
    case 2: handleList();   break;
    case 3: handleUpdate(); break;
    case 4: handleDelete(); break;
    default:
        m_view.showMessage("잘못된 입력입니다.");
        break;
    }
    ConsoleUtil::printSeparator();
}

void SampleController::handleCreate()
{
    std::string name = m_view.promptName();
    std::string desc = m_view.promptDescription();

    m_repo.add(Sample{ 0, name, desc });
    m_view.showMessage("샘플이 등록되었습니다.");
}

void SampleController::handleList()
{
    const auto samples = m_repo.findAll();
    if (samples.empty())
    {
        m_view.showMessage("등록된 샘플이 없습니다.");
        return;
    }
    m_view.showSamples(samples);
}

void SampleController::handleUpdate()
{
    const auto samples = m_repo.findAll();
    if (samples.empty())
    {
        m_view.showMessage("수정할 샘플이 없습니다.");
        return;
    }
    m_view.showSamples(samples);

    const int id = m_view.promptId();
    Sample* found = m_repo.findById(id);
    if (!found)
    {
        m_view.showMessage("해당 ID의 샘플을 찾을 수 없습니다.");
        return;
    }

    m_view.showSample(*found);
    std::string name = m_view.promptName();
    std::string desc = m_view.promptDescription();

    if (m_repo.update(Sample{ found->id, name, desc }))
        m_view.showMessage("샘플이 수정되었습니다.");
    else
        m_view.showMessage("수정에 실패했습니다.");
}

void SampleController::handleDelete()
{
    const auto samples = m_repo.findAll();
    if (samples.empty())
    {
        m_view.showMessage("삭제할 샘플이 없습니다.");
        return;
    }
    m_view.showSamples(samples);

    const int id = m_view.promptId();
    if (m_repo.remove(id))
        m_view.showMessage("샘플이 삭제되었습니다.");
    else
        m_view.showMessage("해당 ID의 샘플을 찾을 수 없습니다.");
}
