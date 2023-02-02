#include "inifile/ini_file.h"
#include "logger/logger.h"
#include "xml/xml.h"

#include <doctest/doctest.h>
#include <iostream>

using namespace zel::utility;
using namespace std;

TEST_CASE("testing Class CLogger") {

    CLogger::Instance()->Open("../log/test.log");
    CLogger::Instance()->SetMax(200);

    debug("勇士总冠军");
    warn("warn message");
    info("info message");
    fatal("fatal message");
}

TEST_CASE("testing Class CIniFile") {
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
}

TEST_CASE("testing Class CXml") {

    CXml s1;

    s1.SetName("student");
    s1.SetAttr("name", "curry");
    s1.SetText("勇士总冠军");

    CXml root;
    root.SetName("students");
    root[0] = s1;

    cout << root.AsString() << endl;

}