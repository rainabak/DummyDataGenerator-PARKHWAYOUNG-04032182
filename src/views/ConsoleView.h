#pragma once
#include <string>

class ConsoleView
{
public:
    void showMainMenu()                          const;
    void showGenerateMenu()                      const;
    void showResult(const std::string& target,
                    int                count,
                    const std::string& filePath) const;
    void showError(const std::string& message)  const;
    void showOverwriteWarning()                  const;

    int  readMainMenuChoice()                                    const;
    int  readMenuChoice()                                        const;
    int  readCount(int max, const std::string& target = "")     const;
    bool readOverwrite()                                         const;
    void pause()                                                 const;

private:
    int readIntInRange(int min, int max) const;
};
