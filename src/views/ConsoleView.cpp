#include "ConsoleView.h"
#include <iostream>

void ConsoleView::showMainMenu() const
{
}

void ConsoleView::showGenerateMenu() const
{
}

void ConsoleView::showResult(const std::string& target,
                              int                count,
                              const std::string& filePath) const
{
}

void ConsoleView::showError(const std::string& message) const
{
}

void ConsoleView::showOverwriteWarning() const
{
}

int ConsoleView::readMenuChoice() const
{
    return 0;
}

int ConsoleView::readCount(int max) const
{
    return 0;
}

bool ConsoleView::readOverwrite() const
{
    return false;
}
