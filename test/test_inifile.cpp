#include "inifile/ini_file.h"

#include <iostream>

using namespace zel::utility;

int main() {

    CIniFile IniFile;
    IniFile.Load("../config/main.ini");

    const std::string& ip = IniFile["mysql"]["ip"];
    int port = IniFile["mysql"]["port"];

    std::cout << "ip: " << ip << "\nport: " << port << "\n";

    IniFile.Set("server", "timeout", 1000);
    IniFile.Set("server", "flag", true);

    bool b = IniFile.Has("server", "flag");
    bool w = IniFile.Has("mysql");

    IniFile.Remove("server", "flag");

    IniFile.Show();

    if (!IniFile.Save("../config/temp.ini"))
        printf("Save Failed!\n");

    return 0;
}
