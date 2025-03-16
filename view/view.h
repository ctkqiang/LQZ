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
    static const std::string CYAN;

   public:
    void DisplayHeader();
    void DisplayHelp();
    void ShowDeviceStatus(bool connected, const std::string& model = "");
    void ShowProgress(const std::string& operation, int percentage);
    void ShowError(const std::string& message);
    void ShowSuccess(const std::string& message);
    void ShowMessage(const std::string& message, const std::string& color);
    void GetInput(const std::string& prompt, std::string& input);
    void ClearScreen();

   private:
    std::string GetOSType();
};

#endif