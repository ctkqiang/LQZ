#ifndef VIEW_H
#define VIEW_H

#include <string>

class View {
   public:
    static const std::string RED;
    static const std::string BLUE;
    static const std::string GREEN;
    static const std::string YELLOW;
    static const std::string WHITE;

   

    void DisplayHeader();
    void DisplayHelp();
    void ShowDevice(const std::string& model);
    void ShowError(const std::string& message);
    void ShowMessage(const std::string& message, const std::string& color);
    void ClearScreen();
    void GetInput(const std::string& prompt, std::string& input);

   private:
    std::string GetOSType();
};

#endif